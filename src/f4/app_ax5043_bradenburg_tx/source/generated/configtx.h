// Copyright (c) 2018 Brandenburg Tech, LLC
// All right reserved.
//
// THIS SOFTWARE IS PROVIDED BY BRANDENBURG TECH, LLC AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRANDENBURT TECH, LLC
// AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/* Warning: This file is automatically generated by AX-RadioLAB.
Manual changes are overwritten! */

#include "../axradio/axradioinit_p.h"

//#define USE_LCD
//#define USE_DBGLINK
#define USE_COM0
//#define DEBUGMSG 
//#define MCU_SLEEP
#define WTIMER0_CLKSRC	CLKSRC_LPOSC
#define WTIMER0_PRESCALER 0x01
#define WTIMER0_PERIOD    0x0280
#define RADIO_MODE_TX        AXRADIO_MODE_ASYNC_TRANSMIT
extern const struct axradio_address remoteaddr_tx;
extern const struct axradio_address_mask localaddr_tx;
extern const uint8_t demo_packet[6];
extern const uint8_t framing_insert_counter;
extern const uint8_t framing_counter_pos;

extern const uint16_t lpxosc_settlingtime;
