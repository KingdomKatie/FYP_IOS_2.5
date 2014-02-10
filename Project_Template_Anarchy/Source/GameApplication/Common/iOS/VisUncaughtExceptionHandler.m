/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//
//  UncaughtExceptionHandler.m
//  UncaughtExceptions
//
//  Created by Matt Gallagher on 2010/05/25.
//  Copyright 2010 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file, free of charge, in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

#import <Vision/Runtime/Common/iOS/VisUncaughtExceptionHandler.h>
#include <execinfo.h>

extern bool VisionDeInitFunction();

NSString * const UncaughtExceptionHandlerSignalExceptionName = @"UncaughtExceptionHandlerSignalExceptionName";
NSString * const UncaughtExceptionHandlerSignalKey = @"UncaughtExceptionHandlerSignalKey";
NSString * const UncaughtExceptionHandlerAddressesKey = @"UncaughtExceptionHandlerAddressesKey";

const int MessageDisplayDurationSeconds = 5;

const NSInteger UncaughtExceptionHandlerSkipAddressCount = 2;
const NSInteger UncaughtExceptionHandlerReportAddressCount = 10;//5;

@implementation UncaughtExceptionHandler

+ (NSArray *)backtrace
{
  void* callstack[128];
  int frames = backtrace(callstack, 128);
  char **strs = backtrace_symbols(callstack, frames);
  
  int i;
  NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
  for (i = UncaughtExceptionHandlerSkipAddressCount;
       i < UncaughtExceptionHandlerSkipAddressCount +
       UncaughtExceptionHandlerReportAddressCount;
       i++)
  {
	 	[backtrace addObject:[NSString stringWithUTF8String:strs[i]]];
  }
  free(strs);
  
  return backtrace;
}

- (void)alertView:(UIAlertView *)anAlertView clickedButtonAtIndex:(NSInteger)anIndex
{
	dismissed = YES;
}

- (void)EmergencyShutdown
{
  // disable signal handlers
  NSSetUncaughtExceptionHandler(NULL);
	signal(SIGABRT, SIG_DFL);
	signal(SIGILL, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
	signal(SIGBUS, SIG_DFL);
  
  // shutdown engine
  VisionDeInitFunction();
  
  // kill app
  _exit(0);
}

// common function for handling exception - display info box and possibly try to continue
- (void)handleException:(NSException *)exception
{
  // log!
  NSString* logErrorDesc = [NSString stringWithFormat:@"--- FATAL ERROR: %@\n%@", [exception reason], [exception userInfo]];
  NSLog(@"%@", logErrorDesc);
  printf("%s", [logErrorDesc UTF8String]);
  
  // message
	UIAlertView *alert =
  [[UIAlertView alloc]
    initWithTitle:@"Unhandled exception"
    message:[NSString stringWithFormat:@"%@\nCallstack:\n%@", [exception reason], [exception userInfo]]
    delegate:self
    cancelButtonTitle:[NSString stringWithFormat:@"Quits after %d seconds", MessageDisplayDurationSeconds]
    otherButtonTitles:nil];
	[alert show];
  
  // start background thread that will kill this app in n seconds
  dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, (unsigned long)NULL), ^(void) {
    [NSThread sleepForTimeInterval:MessageDisplayDurationSeconds];
    [self EmergencyShutdown];
  });

  // try to run the mainloop - this way the messagebox will be displayed at least once (most likely the app will be hanging afterwards)
	CFRunLoopRef runLoop = CFRunLoopGetCurrent();
	CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
	while (!dismissed)
	{
		for (NSString *mode in (NSArray *)allModes)
			CFRunLoopRunInMode((CFStringRef)mode, 0.001, false);
	}
  
  // cleanup (unlikely that we'll ever come here)
	CFRelease(allModes);
  [self EmergencyShutdown];
}

@end

// Handler for exceptions (unlikely since exceptions are off in the c++ part)
void HandleException(NSException *exception)
{	
	NSArray *callStack = [UncaughtExceptionHandler backtrace];
	NSMutableDictionary *userInfo =
  [NSMutableDictionary dictionaryWithDictionary:[exception userInfo]];
	[userInfo setObject:callStack forKey:UncaughtExceptionHandlerAddressesKey];
	
	[[[[UncaughtExceptionHandler alloc] init] autorelease]
   performSelectorOnMainThread:@selector(handleException:)
   withObject:
   [NSException
    exceptionWithName:[exception name]
    reason:[exception reason]
    userInfo:userInfo]
   waitUntilDone:YES];
}

// Handler for signals (like segfault etc.)
void SignalHandler(int signal)
{
  NSArray *callStack = [UncaughtExceptionHandler backtrace];
	NSMutableDictionary *userInfo =
  [NSMutableDictionary dictionaryWithObject:[NSNumber numberWithInt:signal]
   forKey:UncaughtExceptionHandlerSignalKey];
  
	[userInfo setObject:callStack forKey:UncaughtExceptionHandlerAddressesKey];
	
  NSString* signalDescription = @"Signal: ";
  signalDescription = [signalDescription stringByAppendingString: [[NSString alloc] initWithUTF8String:strsignal(signal)]];
  
	[[[[UncaughtExceptionHandler alloc] init] autorelease]
   performSelectorOnMainThread:@selector(handleException:)
   withObject:
   [NSException
    exceptionWithName:UncaughtExceptionHandlerSignalExceptionName
    reason: signalDescription
    userInfo: userInfo]
   waitUntilDone:YES];
}

// registers exception handling
void InstallUncaughtExceptionHandler()
{
	NSSetUncaughtExceptionHandler(&HandleException);
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGBUS, SignalHandler);
}

/*
 * Havok SDK - Base file, BUILD(#20131218)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
