# STM32 Microsecond Timing Framework
**Architecture Specification**

Version: 0.1

Author:
- Xiong Danfeng
- OpenAI ChatGPT (Design Review)

---

# 1. Goals

This project provides a reusable framework for implementing deterministic
microsecond timing protocols on STM32 MCUs.

The framework is designed around the following principles:

- Deterministic timing
- Interrupt driven
- No busy waiting
- No HAL dependency
- CMSIS only
- Unit test friendly
- SafeRTOS compatible
- Portable
- DMA ready
- Zero dynamic memory allocation

The first protocol implemented using this framework is the MAX17215
Single-Wire Interface (SWI).

The framework itself is protocol independent.

---

# 2. Non Goals

The framework is NOT intended to be:

- a generic RTOS
- a task scheduler
- a software timer library
- a protocol stack

Only deterministic microsecond protocol execution is supported.

---

# 3. Design Philosophy

The framework follows one simple rule:

> The timer executes callbacks at deterministic timestamps.

Everything else is built on top of this rule.

The framework never delays execution.

Instead every protocol state schedules the next timestamp.

---

# 4. Layer Architecture

+------------------------------+
| Application                  |
+--------------+---------------+
               |
               v
+------------------------------+
| Device Driver                |
| (MAX17215, ...)              |
+--------------+---------------+
               |
               v
+------------------------------+
| Protocol FSM                 |
| (SWI, UART, ...)             |
+--------------+---------------+
               |
               v
+------------------------------+
| Executor                     |
+--------------+---------------+
               |
               v
+------------------------------+
| Timer Backend                |
+--------------+---------------+
               |
               v
+------------------------------+
| STM32 Hardware               |
+------------------------------+

---

# 5. Module Responsibilities

## Executor

Responsible for

- absolute timestamps
- callback execution
- timer arming

Executor SHALL NOT

- know GPIO
- know protocols
- know MAX17215
- allocate memory

---

## Timer Backend

Responsible for

- TIM12 configuration
- current timestamp
- compare register
- interrupt handling

Timer Backend SHALL NOT

- know protocol
- know GPIO semantics
- know MAX17215

---

## Protocol FSM

Responsible for

- protocol timing
- protocol states
- GPIO actions

Protocol FSM SHALL NOT

- configure timer
- access registers directly
- busy wait

---

## Device Driver

Responsible for

- register map
- CRC
- retry policy
- public API

Device Driver SHALL NOT

- manipulate GPIO
- manipulate timer

---

# 6. Timing Model

The framework uses

absolute timestamps

instead of

relative delays.

Example

Incorrect

Delay(10us)

Correct

Execute at T + 10us

This eliminates accumulated timing drift.

---

# 7. Execution Model

One callback is active at any time.

callback()

↓

perform one protocol state

↓

schedule next callback

↓

return

The callback must execute quickly.

Callbacks shall never block.

---

# 8. GPIO Model

The framework never accesses GPIO directly.

Instead an abstract bus interface is used.

Example

Bus_DriveLow()

Bus_Release()

Bus_Read()

The protocol layer never knows the MCU.

---

# 9. Memory Policy

Dynamic allocation is forbidden.

The framework uses

- stack
- static objects
- caller owned objects

Only.

---

# 10. Interrupt Policy

Interrupt handlers must be minimal.

Allowed

- clear flags
- execute callback
- arm next compare

Forbidden

- loops
- delays
- protocol logic
- malloc
- printf

---

# 11. Blocking API

Public APIs may block.

Internal callbacks shall never block.

Example

Application

↓

MAX17215_ReadRegister()

↓

wait completion

↓

Executor callbacks

↓

transaction complete

↓

return

---

# 12. Error Handling

Every public function returns status.

Example

OK

TIMEOUT

CRC_ERROR

BUS_ERROR

INVALID_PARAMETER

---

# 13. Unit Testing

Every module except Timer Backend shall compile
on Windows.

Timer Backend shall be replaced by a mock implementation.

The same protocol code must execute unchanged
on STM32 and Windows.

---

# 14. DMA Compatibility

The architecture is designed so that the timer backend
may later be replaced by

TIM12 Interrupt

or

TIM12 + DMA

without changing

- protocol FSM
- device driver
- public API

---

# 15. Coding Rules

C99

CMSIS only

No HAL

No global mutable state

One module = one responsibility

One public header per module

Private functions declared static

All interfaces documented

No hidden side effects

---

# 16. Future Extensions

The framework is intended to support

- Software UART
- WS2812
- DHT22
- 1-Wire
- Custom timing protocols

without modification of the executor.

---

# 17. Current Status

Architecture

✓ Frozen

Implementation

In Progress