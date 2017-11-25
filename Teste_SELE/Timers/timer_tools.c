/* Copyright (c) 2010  Paulo Costa
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <ctype.h>
#include "timer_tools.h"

// calculated constants
#define	TIMER_TICKS			   ((uint8_t)(((double)F_CPU/PRESCALER)/(CTRL_FREQ*1000)))


volatile uint16_t tick_count;

// ----------------------------------------------------
//    1kHz Interrupt Routine

ISR(TIMER0_OVF_vect)
{
  // reset timer0 count to give CTRL_FREQ frequency
  TCNT0 = TCNT0 + (256 - TIMER_TICKS);

  tick_count++;
  /*if (tick_count & 0x80) {
    port_bit_set(LED);
  } else {
    port_bit_clear(LED);
  }*/
}

void init_mili_timers(void)
{
  // reset timer0 count
  TCNT0 = 0;
  // set timer0 prescaler to clk/8
#if PRESCALER == 1
  TCCR0B = 1;
#elif PRESCALER == 8
  TCCR0B = 2;
#elif PRESCALER == 64
  TCCR0B = 3;
#else
#error PRESCALER value not suported
#endif
  // clear pending interrupts
  TIFR0 |= (1 << TOV0);
  // enable timer0 overflow interrupt
  TIMSK0 |= (1 << TOIE0);
}

// ----------------------------------------------------
// Timers



void start_timer(mili_timer* timer, uint16_t ms)
{
  timer->start_value = get_ms_count();
  timer->delta = ms;
}

uint16_t get_timer_time(mili_timer* timer)
{
  return elapsed_time_ms(timer->start_value);
}


uint8_t get_timer(mili_timer* timer)
{
  return check_elapsed_time_ms(timer->start_value, timer->delta);
}


uint16_t get_ms_count(void)
{
  uint16_t my_tick_count;
  ATOMIC_BLOCK(ATOMIC_FORCEON){
    my_tick_count = tick_count;
  }
  return my_tick_count;
}


uint16_t elapsed_time_ms(uint16_t start_count)
{
  return get_ms_count() - start_count;
}

uint16_t check_elapsed_time_ms(uint16_t start_count, uint16_t total_count)
{
  if( (get_ms_count() - start_count) > total_count ) return 1;
  else return 0;
}

