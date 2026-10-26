# libgeneric
A collection of generic C++ code for Arduino based micro controller projects. 

See [CHANGELOG.md](CHANGELOG.md) for the changes of each version.

## Usage

Add the library to the `lib_deps` of your `platformio.ini`:

```
lib_deps = fjulian79/libgeneric@^2.2.0
```

v2.x is not compatible with v1.x, see [Migration from v1.x](#migration-from-v1x).
Requires the Arduino framework and at least C++11, tested on ESP32 and on the
Raspberry Pi Pico using the core of Earle Philhower.

The [examples](examples) show the typical use of `Task` and `UpTime`, `Fifo`
between an interrupt and the main loop, `LineRingBuffer` as log, `SysLog` and
`EventLog` and `crc8`.

## Content

| Header | Content |
|---|---|
| `generic/generic.hpp` | `constexpr` helpers like `generic::clamp()`, `generic::mapRange()`, `generic::toRadians()`, the constant `generic::pi` and the macros `arraysize`, `container_of` and `unused` |
| `generic/ringbuffer.hpp` | `RingBuffer`, byte ring buffer on a buffer provided by the user |
| `generic/fifo.hpp` | `Fifo`, byte FiFo based on `RingBuffer` |
| `generic/lineringbuffer.hpp` | `LineRingBuffer`, stores lines of text, e.g. for logs or a history |
| `generic/crc8.hpp` | `crc8`, CRC8 calculation including parameter sets of well known algorithms |
| `generic/task.hpp` | `Task`, runs code periodically based on a millisecond tick |
| `generic/uptime.hpp` | `UpTime`, 64 bit uptime in milliseconds and a printable uptime string |
| `generic/syslog.hpp` | `SysLog`, system log with timestamps and syslog severities, optionally kept across a reset |
| `generic/eventlog.hpp` | `EventLog`, log of events of one kind with timestamps, e.g. received codes, optionally printed live |
| `generic/timestamp.hpp` | `timeStampMillis()` and `timeStampSystem()`, timestamps for the logs or any `Print` |
| `generic/boundedwriter.hpp` | `BoundedWriter`, a `Print` writing into a fixed buffer, which can not overflow |

### Things to know

- **generic.hpp** keeps the type of the first argument, e.g. 
  `generic::clamp(myUint8, 0, 100)` returns an `uint8_t` and 
  `generic::toRadians(1.0f)` calculates in `float`, not in `double`. Its names
  differ from the macros defined by Arduino like `constrain()` or `PI`, so 
  both can be used together.
- **Fifo and RingBuffer** can be used by one producer and one consumer 
  concurrently, e.g. an interrupt and the main loop, without locking. One byte
  of the buffer is always kept free for this.
- **LineRingBuffer** removes the oldest lines as needed. Positions returned by
  `first()`, `next()` etc. are only valid until the next `append()`. It is not
  safe for concurrent use. Its state can be saved by `getState()` and restored
  by `setState()`, e.g. to keep a log in RTC memory across a reset.
- **crc8** with the default parameters calculates CRC-8/SMBUS. Some of the
  `CRC8_POLY_*` polynoms need further parameters to match the standard, use 
  the parameter sets in this case, e.g. `crc8 crc(CRC8_MAXIM)`.
- **Task** is scheduled the first time one interval after tick 0. If it is 
  scheduled late, the next interval starts at the time it actually ran. The 
  overflow of `millis()` after 49.7 days is handled.
- **UpTime** keeps counting across the overflow of `millis()`, as long as 
  `loop()` is called at least once within 49.7 days.
- **SysLog** stores messages up to the log level, info by default, and prints
  the ones up to the console level, err by default, to a console, Serial by
  default. The memory is provided by the user as `SysLogMemory<N>`, placed in
  memory which is not initialized on startup it is kept across a reset, e.g.
  by a watchdog. Without memory messages are only printed. Messages longer
  than 127 characters are truncated. Not safe for concurrent use.
- **EventLog** works like SysLog without severities. New events can be printed
  to a monitor as they occur, e.g. to follow received codes like `tail -f`.
- **Timestamps** of both logs are printed by a function set by
  `setTimeStamp()`, without one there is no timestamp. `timeStampMillis()`
  works everywhere, but wraps after 49.7 days. `timeStampSystem()` is only
  available on ESP32 and Raspberry Pi Pico, it prints the time since boot
  starting at 1970-01-01 until the time is set, e.g. by NTP, the local time
  afterwards. Functions not used are not linked, e.g. the time zone handling.
- **BoundedWriter** drops what does not fit into its buffer and tells so by
  `overflowed()`, it neither writes behind the end nor uses the heap. The logs
  use it to hand the space of the timestamp to the timestamp function, but it
  is useful on its own, e.g. to compose text without `String`.

## Migration from v1.x

v2.0.0 removed the macros of `generic.hpp` which collided with the standard 
library or with Arduino:

| v1.x macro | Replacement |
|---|---|
| `min(a, b)`, `max(a, b)` | `std::min(a, b)`, `std::max(a, b)` from `<algorithm>` |
| `abs(x)` | `std::abs(x)` from `<cstdlib>` |
| `round(x)` | `std::lround(x)` from `<cmath>`, returns a `long` like the macro |
| `offsetof(st, m)` | `offsetof(st, m)` from `<stddef.h>`, included by `generic.hpp` |
| `constrain(x, l, h)` | `generic::clamp(x, l, h)` |
| `map(x, inMin, inMax, outMin, outMax)` | `generic::mapRange(x, inMin, inMax, outMin, outMax)` |
| `sq(x)` | `generic::square(x)` |
| `wrapInc(x, n, s)` | `generic::wrapInc(x, n, s)` |
| `radians(d)`, `degrees(r)` | `generic::toRadians(d)`, `generic::toDegrees(r)`, floating point only |
| `PI`, `HALF_PI`, `TWO_PI` | `generic::pi`, `generic::halfPi`, `generic::twoPi` |
| `DEG_TO_RAD`, `RAD_TO_DEG` | `generic::toRadians()`, `generic::toDegrees()` |
| `breakIfDiverse(a, b)` | `if (a != b) break;` |

In Arduino projects the macros `constrain()`, `map()`, `sq()`, `radians()`,
`degrees()`, `PI` etc. are still provided by `Arduino.h`.

## Tests

Unit tests for the platform independent parts can be run on the host, using 
AddressSanitizer and UndefinedBehaviorSanitizer:

```
pio test -e native
```

`UpTime`, the logs, `timeStampMillis()` and `BoundedWriter` are tested using a
minimal Arduino replacement in `test/support`. `timeStampSystem()` depends on
the system time of the platform and is not tested on the host.
