# libgeneric
A collection of generic and platform independent code. 

See [CHANGELOG.md](CHANGELOG.md) for the changes of each version.

## Usage

Add the library to the `lib_deps` of your `platformio.ini`:

```
lib_deps = https://github.com/fjulian79/libgeneric.git
```

## Content

| Header | Content |
|---|---|
| `generic/generic.hpp` | Macros like `min`, `max`, `constrain`, `map`, `arraysize`, `container_of` and math constants |
| `generic/ringbuffer.hpp` | `RingBuffer`, byte ring buffer on a buffer provided by the user |
| `generic/fifo.hpp` | `Fifo`, byte FiFo based on `RingBuffer` |
| `generic/lineringbuffer.hpp` | `LineRingBuffer`, stores lines of text, e.g. for logs or a history |
| `generic/crc8.hpp` | `crc8`, CRC8 calculation including parameter sets of well known algorithms |
| `generic/task.hpp` | `Task`, runs code periodically based on a millisecond tick |
| `generic/uptime.hpp` | `UpTime`, 64 bit uptime and a printable uptime string, needs Arduino |

### Things to know

- **Macros evaluate their arguments more than once.** Don't pass expressions 
  with side effects, e.g. `max(i++, 0)` increments `i` twice.
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

## Tests

Unit tests for the platform independent parts can be run on the host, using 
AddressSanitizer and UndefinedBehaviorSanitizer:

```
pio test -e native
```

`UpTime` is tested using a minimal Arduino replacement in `test/support`.
