/*
 * main.c -- Main program for the GoAhead WebServer (RTEMS version)
 *
 * Copyright (c) Go Ahead Software Inc., 1995-1999. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 *  $Id: webmain.c,v 1.2 2001-09-27 12:02:00 chris Exp $
 */

/******************************** Description *********************************/

/*
 *	Main program for for the GoAhead WebServer. This is a demonstration
 *	main program to initialize and configure the web server.
 */

/********************************* Includes ***********************************/

#include	"uemf.h"
#include	"wsIntrn.h"
#include	<signal.h>
#include	<sys/time.h>
#include 	<pthread.h>

#include	<rtems/error.h>

/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */

static char_t		*rootWeb = T("web");			/* Root web directory */
static char_t		*password = T("");				/* Security password */
static int			port = 80;						/* Server port */
static int			retries = 5;					/* Server port retries */
static int			finished;						/* Finished flag */

/*
 *	Structure to hold timer events
 */
typedef struct {
	void	(*routine)(long arg);	/* Timer routine */
	long	arg;					/* Argument to routine */
} websTimer_t;

/* The following holds the pointer to an allocated websTimer_t structure .
 * Using this method only one timer can be active at a time, but
 * for the WebServer, this should be OK. 
 */
websTimer_t *tp;

/****************************** Forward Declarations **************************/

static int 	initWebs();
static int	aspTest(int eid, webs_t wp, int argc, char_t **argv);
static void formTest(webs_t wp, char_t *path, char_t *query);
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t* url, char_t* path, char_t* query);
static void timerProc(int signo);
#if B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif
static timer_t timer_id;
static void rtems_httpd_daemon();
 
/*********************************** Code *************************************/
/*
 *	Main -- entry point from RTEMS
 */
int rtems_initialize_webserver()
{
  rtems_status_code   sc;
  rtems_id            tid;
  int		      priority;

  /***********************************************************************
   * Default HTTPD priority.
   **********************************************************************/
  priority = 40;
  
  sc = rtems_task_create(rtems_build_name('H', 'T', 'P', 'D'),
			 priority, 8*1024,
			 RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_NO_ASR |
			 RTEMS_INTERRUPT_LEVEL(0),
			 RTEMS_NO_FLOATING_POINT | RTEMS_LOCAL,
			 &tid);
   if (sc != RTEMS_SUCCESSFUL)
   {
      return(RTEMS_UNSATISFIED);
   }

   sc = rtems_task_start(tid, rtems_httpd_daemon, 0);
   if (sc != RTEMS_SUCCESSFUL)
   {
      return(RTEMS_UNSATISFIED);
   }   

   return(RTEMS_SUCCESSFUL);

}

static void
rtems_httpd_daemon()
{
/*
 *	Initialize the memory allocator. Allow use of malloc and start with a 
 *	10K heap.
 */
	bopen(NULL, (10 * 1024), B_USE_MALLOC);

/*
 *	Initialize the web server
 */
	if (initWebs() < 0) {
	  rtems_panic("Unable to initialize Web server !!\n");
	}

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */
	while (!finished) {
	  if (socketReady() || socketSelect()) {
			socketProcess();
	  }
	}

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
	websCloseServer();
	socketClose();
#if B_STATS
	memLeaks();
#endif
	bclose();
        rtems_task_delete( RTEMS_SELF );
}

/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs()
{
	struct hostent*	hp;
	struct in_addr	intaddr;
	char			host[128], dir[128], webdir[128];
	char			*cp;
	char_t			wbuf[128];

/*
 *	Initialize the socket subsystem
 */
	socketOpen();

/*
 *	Define the local Ip address, host name, default home page and the 
 *	root web directory.
 */
	if (gethostname(host, sizeof(host)) < 0) {
		error(E_L, E_LOG, T("Can't get hostname"));
		return -1;
		}
	
/*	intaddr.s_addr = (unsigned long) hostGetByName(host); */
	if ((hp = gethostbyname(host)) == NULL) {
		error(E_L, E_LOG, T("Can't get host address"));
		return -1;
	}
	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
		(size_t) hp->h_length);

/*
 *	Set ../web as the root web. Modify this to suit your needs
 */
	getcwd(dir, sizeof(dir)); 
	if ((cp = strrchr(dir, '/'))) {
		*cp = '\0';
	}
	sprintf(webdir, "%s/%s", dir, rootWeb);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultDir(webdir);
	ascToUni(wbuf, inet_ntoa(intaddr), sizeof(wbuf));
	websSetIpaddr(wbuf);
	ascToUni(wbuf, host, sizeof(wbuf));
	websSetHost(wbuf);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultPage(T("default.asp"));
	websSetPassword(password);

/* 
 *	Open the web server on the given port. If that port is taken, try
 *	the next sequential port for up to "retries" attempts.
 */
	websOpenServer(port, retries);

/*
 * 	First create the URL handlers. Note: handlers are called in sorted order
 *	with the longest path handler examined first. Here we define the security 
 *	handler, forms handler and the default web page handler.
 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler, 
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler, 
		WEBS_HANDLER_LAST); 

/*
 *	Now define two test procedures. Replace these with your application
 *	relevant ASP script procedures and form functions.
 */
	websAspDefine(T("aspTest"), aspTest);
	websFormDefine(T("formTest"), formTest);

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0); 
	return 0;
}

/******************************************************************************/
/*
 *	Test Javascript binding for ASP. This will be invoked when "aspTest" is
 *	embedded in an ASP page. See web/asp.asp for usage. Set browser to 
 *	"localhost/asp.asp" to test.
 */

static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name, *address;

	if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	return websWrite(wp, T("Name: %s, Address %s"), name, address);
}
/******************************************************************************/
/*
 *	Test form for posted data (in-memory CGI). This will be called when the
 *	form in web/asp.asp is invoked. Set browser to "localhost/asp.asp" to test.
 */

static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t	*name, *address;

	name = websGetVar(wp, T("name"), T("Joe Smith")); 
	address = websGetVar(wp, T("address"), T("1212 Milky Way Ave.")); 

	websHeader(wp);
	websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
	websFooter(wp);
	websDone(wp, 200);
}

/******************************************************************************/
/*
 *	Create a timer to invoke the routine in "delay" milliseconds.
 */

void *emfCreateTimer(int delay, void (*routine)(long arg), 	long arg)
{
/* this variable is only used in the if'ed 0 section below */
#if 0
	struct sigaction	act;
#endif
	struct itimerspec its = { {0,0}, {0,0} };
	struct sigevent se;
	int 	status;

	if ((tp = balloc(B_L, sizeof(websTimer_t)))) {
		tp->routine = routine;
		tp->arg = arg;
	}
	else {
		return NULL;
	}

	se.sigev_notify = SIGEV_THREAD;
	se.sigev_value.sival_ptr = tp;
	se.sigev_notify_function = (void (*)(union sigval)) timerProc;

	/*
	 * NOT POSIX?
	 * se.sigev_notify_attributes = NULL;
	 */


	status = timer_create(CLOCK_REALTIME, &se, &timer_id);
	if (status != 0) {
		bfree(B_L, tp);
		return NULL;
	}
	/* convert delay millisecs to secs and usecs required by struct */
	its.it_value.tv_sec = delay / 1000;
	its.it_value.tv_nsec = (delay % 1000) * 1000000;

	status = timer_settime(timer_id, 0, &its, 0);
	if (status != 0) {
		bfree(B_L, tp);
		return NULL;
	}
	
#if 0
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = timerProc;
	sigaction(SIGALRM, &act, NULL);

	/* convert delay millisecs to secs and usecs required by struct */
	its.it_value.tv_sec = delay / 1000;
	its.it_value.tv_usec = (delay % 1000) * 1000;

	if (setitimer(ITIMER_REAL, &its, NULL) ==  -1) {
		bfree(B_L, tp);
		return NULL;
	}
#endif
	return tp;
}

/******************************************************************************/
/*
 *	Delete a timer
 */

void emfDeleteTimer(void * id)
{
	websTimer_t *wtp;
	/*struct itimerval its = { {0,0}, {0,0} };*/

	wtp = (websTimer_t *)id;
	/*	setitimer(ITIMER_REAL, &its, NULL);*/
	timer_delete(timer_id);
	bfree(B_L, wtp);
}

/******************************************************************************/
/*
 *	Timer handler
 */

static void timerProc(int signo)
{
	websTimer_t wtp = *tp;

/* Copy the timer structure to a local first and delete it before calling
 * the function, since the function could create another timer.  In this 
 * implementation, only one timer can be allocated at a time.
 */

	bfree(B_L, tp);
	(wtp.routine)(wtp.arg);
}

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t* url, char_t* path, char_t* query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
		websRedirect(wp, T("home.asp"));
		return 1;
	}
	return 0;
}

/******************************************************************************/

#if B_STATS
static void memLeaks() 
{
	int		fd;

	if ((fd = gopen(T("leak.txt"), O_CREAT | O_TRUNC | O_WRONLY)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif

/******************************************************************************/
