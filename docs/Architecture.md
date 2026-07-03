# STM32 Microsecond Timing Framework
**Architecture Specification**

Version: 0.2

Author:
- Xiong Danfeng
- OpenAI ChatGPT (Design Review)

---
# 0. Folder Structure
stm32-swi-max17215/
│
├── docs/
│      Architecture.md  //done
│      Timing.md   // empty
│      CodingStyle.md    // empty
|      DesignDecisions.md  // empty
├── rfc/
|      RFC-0001-Executor.md  // empty
│      RFC-0002-SWI.md   // empty
│      RFC-0003-MAX17215.md    // empty
├── images/
│      architecture.drawio   // empty
│      swi_timing.drawio  // empty
├── Drivers/
│      Common/
│          critical_section.h   // done
│          critical_section.c   // done
│      Executor/
│          executor.h   // done
│          executor.c  // done
│
│      Bus/
│          bus.h   // empty
│          bus_stm32f4.c   // empty
│          bus_mock.cpp   // empty
│
│      SWI/
│          swi.h  // empty
│          swi.c    // empty
│
│      Max17215/
│          max17215.h   // done
│          max17215.c   // done
│
│      BSP/
│          tim12_backend.c   // done
│          tim12_backend.h   // done
│          executor_backend.h   // done
│          executor_backend_mock.c  // done
│          time_utils.h      // done
├── Tests/
│          mock_backend.c
│          battery_test.cpp    // done
│          executor_test.cpp   // empty
│          max17215_test.cpp   // empty
│          swi_test.cpp    // empty
│          
│          
│          
├── Example/
│
└── CMakeLists.txt


    
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

DMA readiness means:

- no protocol depends on CPU timing loops
- all timing derived from scheduled timestamps
- bus operations can be replaced by DMA descriptors
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

# 3.1 Design Invariants

The following rules define the architecture and shall not be violated.

## I1. Single Active Transaction

At most one protocol transaction may be active.

Reason

Protocols supported by this framework are inherently sequential.
This simplifies timing, testing and future DMA migration.

---

## I2. Single Pending Callback

Executor manages at most one pending callback.

Reason

Each callback schedules the next callback.

This avoids event queues and guarantees deterministic memory usage.

---

## I3. Executor is Protocol Independent

Executor shall never know

- GPIO
- SWI
- MAX17215
- protocol semantics

---

## I4. Timer Backend is Hardware Specific

Timer Backend is the only module allowed to access

TIM12
NVIC
CMSIS timer registers

---

## I5. Protocol FSM Owns Timing

Only the protocol FSM decides

- next deadline
- GPIO action
- state transition

---

## I6. No Busy Waiting

No protocol implementation shall delay execution.

Timing is always interrupt driven.

---

## I7. No Dynamic Memory

The framework shall never allocate memory dynamically.

---

## I8. Callbacks Execute to Completion

Callbacks shall never block,
sleep,
or wait for external events.

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

4.1. Ownership Diagram

Application
  owns
MAX17215
  owns
SWI FSM
  owns
Executor
  uses
Timer Backend (singleton, BSP-owned)

4.2. State Machine Diagrams

IDLE
 │
 ▼
RESET_LOW
 │
 ▼
RESET_RELEASE
 │
 ▼
WRITE_BIT_LOW
 │
 ▼
WRITE_BIT_RELEASE
 │
 ▼
NEXT_BIT
 │
 ├── more bits → WRITE_BIT_LOW
 │
 └── done → COMPLETE

 4.3 Timing Diagrams
Bus
────┐      ┌────────────────────────────
    │      │
    └──────┘
    <6µs>

Sample
───────────────▲────────────────────────
               15µs
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


Timing accuracy

Timer resolution

<= 1 us

Jitter

<= 1 us

ISR execution

< 3 us

Protocol callback

< 5 us

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

# 9 Concurrency Model

The framework executes in exactly two contexts.

Thread Context

- Public APIs
- Blocking waits
- Driver initialization

Interrupt Context

- Executor callback
- Timer backend

Shared data shall be protected using
minimal critical sections.

Callbacks never preempt each other.

# 10. Memory Policy

Dynamic allocation is forbidden.

The framework uses

- stack
- static objects
- caller owned objects

Only.

---

# 11. Interrupt Policy

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

# 12. Blocking API

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

# 13. Error Handling

## 13.1 definition:
a common status type definition for error model :

typedef enum
{
    STATUS_OK = 0,

    STATUS_BUSY,

    STATUS_TIMEOUT,

    STATUS_CRC_ERROR,

    STATUS_BUS_ERROR,

    STATUS_PARAMETER_ERROR,

    STATUS_INTERNAL_ERROR

} Status;
## 13.2 rule
Every module uses this type.
---

# 14. Unit Testing

Every module except Timer Backend shall compile
on Windows.

Timer Backend shall be replaced by a mock implementation.

The same protocol code must execute unchanged
on STM32 and Windows.

---

# 15. DMA Compatibility

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

# 16. Coding Rules

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

# 17. Portability Rules
STM32 specific code shall exist only inside

Drivers/BSP/

Everything else must compile on Windows.
---

# 18. Interrupt ownership rule

Only Timer Backend may define ISR handlers.

All other modules MUST NOT define ISRs.

This prevents future architecture breakage.

# 19. Re-entrancy rule


Executor callbacks are non-reentrant.

A new callback shall not be triggered
until the current callback completes.

# 20. Atomicity Rule

All shared state between ISR and thread context
must be accessed atomically or within critical sections.

Reason

Prevents race conditions between Executor and Timer Backend.

# 21. Future Extensions

The framework is intended to support

- Software UART
- WS2812
- DHT22
- 1-Wire
- Custom timing protocols

without modification of the executor.

---

# 22. State Ownership
State belongs to the module that owns it.

Example

MAX17215 state

belongs to MAX17215.

SWI state

belongs to SWI.

Executor state

belongs to Executor.

No module may modify another module's state directly.

# 23 Future Multi-Protocol Support

Current implementation

One active protocol

Future

Multiple protocol implementations

Still one active transaction

# 24. Current Status

Architecture

✓ Frozen

Implementation

In Progress