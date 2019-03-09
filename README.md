# Event

C++11 based thread notification object that mimics WINAPI Event Objects.


## Foreword

An event object is a synchronization object useful in sending a signal to a thread indicating that a particular event
has occurred.


## Usage

The Event class mimics WINAPI Event Objects with use of C++11 features. For those familiar with WINAPI events the list
below is a shortcut for all equivalent WINAPI functions:

* `CreateEvent` - Create an object of `Event` class with identical arguments
* `CloseHandle` - Destroy the object instance
* `SetEvent` - call `set()` function on an object instance
* `ResetEvent` - call `reset()` function on an object instance
* `WaitForSingleObject` with `INFINITE` - call `wait()` function on an object instance with no arguments
* `WaitForSingleObject` with timeout - call `wait()` function with timeout specified in function argument
* additionally call `change()` function to switch in runtime event type to either manual or automatic reset

The object is always created with two arguments. The first argument `initial` is to specify initial state of the object.
The state can be either signaled (`true`) or nonsignaled (`false`). The second argument `manual` specifies if the event
is reset manually (`true`) or automatically (`false`).

The event can be in either signaled or nonsignaled state. State of the object can always be explicitly set to signaled
via `set()` or to nonsignaled via `reset()` functions. When the event is nonsignaled every call to `wait()` function
is blocking for as long as the event remains in nonsignaled state. Change of the event state to signaled will
immediately release all threads waiting for the object but only when event is configured for manually reset. When an
event is configured to automatic reset only one of waiting threads is released and then state of the event is
automatically switched back to nonsignaled, and, therefore all other tasks continues waiting for another signalization
(notification).

Additionally the `wait()` function can be specified with timeout as an argument. In this case the `wait()` function is
exited whenever an event is signaled or the timeout is reached.

For a rare cases it is useful to modify event reset type. It can be done at any time via call to `change()` function.
The function takes one argument `manual` which specifies if the event is either reset manually (`true`) or automatically
(`false`).
