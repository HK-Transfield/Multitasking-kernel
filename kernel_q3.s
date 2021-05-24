# Define programmable timer macros
.equ timer_ctrl,    0x72000
.equ timer_load,    0x72001
.equ timer_count,   0x72002
.equ timer_iack,    0x72003 

# Define parallel macros
.equ par_btn,		0x73001
.equ par_ctrl,		0x73004
.equ par_iack,		0x73005
.equ par_ulssd,		0x73006
.equ par_urssd,		0x73007
.equ par_llssd,		0x73008
.equ par_lrssd,		0x73009

# Define serial macros
.equ sp2_tx,		0x71000
.equ sp2_rx,		0x71001
.equ sp2_ctrl,		0x71002
.equ sp2_stat,		0x71003
.equ sp2_iack,		0x71004

######################################################################

.text
.global main
main:

# Adjust the CPU control register to setup interrupts
    movsg $1, $cctrl        # Copy the current value of $cctrl into $1
    andi $1, $1, 0x000f     # Mask (disable) all interrupts
    ori $1, $1, 0x4F        # Enable IRQ2 and IE (global interrupt enable)
    movgs $cctrl, $1        # Copy the new CPU control value back to $cctrl

# Setup a new exception/interrupt handler
    movsg $1, $evec         # Copy the old handler's address to $1
    sw $1, old_vector($0)   # Save it to memory
    la $1, handler          # Get the address of the handler
    movgs $evec, $1         # Copy the address of the handler into $evec

# Setup timer
    sw $0, timer_iack($0)   # Acknowledge any outstanding interrupts
    addui $1, $0, 24        # Put our count value into the timer load register
    sw $1, timer_load($0)    
    addui $1, $0, 0x3       # Enable the timer and set auto-restart mode
    sw $1, timer_ctrl($0)  

serial_task:
    jal serial_main

######################################################################

handler:
    movsg $13, $estat       # The cause of the interrupt
    andi $13, $13, 0x40     # Check if the interrupt is because of the programmable timer (IRQ2)
    bnez $13, handle_irq2   # No other interrupt; the timer caused it
    lw $13, old_vector($0)  # Restore the old old_vector as the handler
    jr $13                  # This will stop counting

handle_irq2:
    sw $0, timer_iack($0)   # Acknowledge the IRQ2 interrupt
    lw $13, counter($0)     # Load counter from memory
    addi $13, $13, 1        # Increment counter by one
    sw $13, counter($0)     # Save new counter value
    rfe                     # Return from interrupt/exception

######################################################################

.bss
old_vector: .word