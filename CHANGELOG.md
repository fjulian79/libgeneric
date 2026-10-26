# Changelog

All notable changes to this project will be documented in this file.
The format is based on Keep a Changelog.

## [v1.3.0] - 2026-10-26
### Added
- Added the `RingBuffer` base class, using indices and providing a non destructive `peek()` as well as saving and restoring its state.
- Added `LineRingBuffer` to store lines of text, e.g. for logs or a history.
- Added reflection and xorout to `crc8` as well as parameter sets of well known algorithms, e.g. `crc8 crc(CRC8_MAXIM)`.
- Added unit tests, run them on the host with `pio test -e native`.
- Added this changelog and a documentation of the content to the README.
### Changed
- `Fifo` is based on `RingBuffer` now, its API is unchanged.
### Fixed
- Fixed `Fifo` reporting a wrong fill level after writing exactly up to the end of the buffer, `getFree()` underflowed.
- Fixed `Fifo::get()` reading behind the buffer after a `read()` ending exactly at the end of the buffer.
- Fixed a race condition in `Fifo`, the consumer reset the write index when the fifo became empty, which could drop data written concurrently by the producer.
- Fixed `CRC8_POLY_DARC`, `EBU`, `ITU`, `MAXIM`, `ROHC` and `WCDMA` not resulting in the standard CRC, use the new parameter sets.
- Fixed assigning a value to `crc8` resetting its parameters to the defaults.
- Fixed missing parentheses in `max()` and `map()`.
- Fixed `degrees()` using an undefined argument name.
- Fixed the millisecond format in `UpTime::toString()`, it printed 4 digits.
- Fixed the format of the days in `UpTime::toString()` on platforms where `uint32_t` is not an `unsigned long`.

## [v1.2.0] - 2023-01-25
### Added
- Added `Task::loop()` to call a task function when scheduled.

## [v1.1.0] - 2023-01-02
### Added
- Added the `UpTime` class.
### Changed
- Renamed `Task_t` to `Task`.

## [v1.0.6] - 2022-03-26
### Added
- Added `setLastTick()` to `Task_t`.

## [v1.0.5] - 2022-02-20
Added the `Task_t` class. First tagged version, providing the generic macros, `Fifo` and `crc8`.
