###################################
# Harmon Transfield
# 1317381
#
# Idle Task
# Multitasking Kernel, COMPX203
###################################



######################################################################
# DECLARE CONSTANTS USING .equ DIRECTIVE
######################################################################

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

# Define PCBs
.equ pcb_link, 0
.equ pcb_reg1, 1
.equ pcb_reg2, 2
.equ pcb_reg3, 3
.equ pcb_reg4, 4
.equ pcb_reg5, 5
.equ pcb_reg6, 6
.equ pcb_reg7, 7
.equ pcb_reg8, 8
.equ pcb_reg9, 9
.equ pcb_reg10, 10
.equ pcb_reg11, 11
.equ pcb_reg12, 12
.equ pcb_reg13, 13
.equ pcb_sp, 14
.equ pcb_ra, 15
.equ pcb_ear, 16
.equ pcb_cctrl, 17



######################################################################
# MAIN ENTRY POINT OF THE MULTITASKING KERNEL
######################################################################

.text
.global main
main:
    subui $sp, $sp, 3
    sw $1, 0($sp)
    sw $2, 1($sp)
    sw $5, 2($sp)

# PCB setup for the serial task process
#########################################	
    addi $5, $0, 0x4D       # Unmask IRQ2,KU=1,OKU=1,IE=0,OIE=1
    
    la $1, serial_pcb     

# Setup the link field    
    la $2, parallel_pcb
    sw $2, pcb_link($1)

# Setup the stack pointer
    la $2, serial_stack
    sw $2, pcb_sp($1)

# Setup the $ear field
    la $2, serial_process
    sw $2, pcb_ear($1)

# Setup $ra for exiting
    la $ra, exit_process
    sw $ra, pcb_ra($1)

# Setup the $cctrl field
    sw $5, pcb_cctrl($1)

# Set first process as current process
    la $1, serial_pcb
    sw $1, current_process($0)
	
# PCB setup for the parallel task process
#########################################	
    addi $5, $0, 0x4D       # Unmask IRQ2,KU=1,OKU=1,IE=0,OIE=1
    
    la $1, parallel_pcb

# Setup the link field    
    la $2, gameSelect_pcb
    sw $2, pcb_link($1)

# Setup the stack pointer
    la $2, parallel_stack
    sw $2, pcb_sp($1)

# Setup the $ear field
    la $2, parallel_process
    sw $2, pcb_ear($1)

# Setup $ra for exiting
    la $ra, exit_process
    sw $ra, pcb_ra($1)

# Setup the $cctrl field
    sw $5, pcb_cctrl($1)

# PCB setup for the gameSelect task process
#########################################	
    addi $5, $0, 0x4D       # Unmask IRQ2,KU=1,OKU=1,IE=0,OIE=1
    
    la $1, gameSelect_pcb

# Setup the link field    
    la $2, serial_pcb
    sw $2, pcb_link($1)

# Setup the stack pointer
    la $2, gameSelect_stack
    sw $2, pcb_sp($1)

# Setup the $ear field
    la $2, gameSelect_process
    sw $2, pcb_ear($1)

# Setup $ra for exiting
    la $ra, exit_process
    sw $ra, pcb_ra($1)

# Setup the $cctrl field
    sw $5, pcb_cctrl($1)

    la $1, gameSelect_pcb
    sw $1, previous_process($0)

# PCB setup for the idle task process
#########################################	
    addi $5, $0, 0x4D       # Unmask IRQ2,KU=1,OKU=1,IE=0,OIE=1
    
    la $1, idle_pcb

# Setup the link field    
    la $2, idle_pcb
    sw $2, pcb_link($1)

# Setup the Multitasking Kernelstack pointer
    la $2, idle_stack
    sw $2, pcb_sp($1)

# Setup the $ear field
    la $2, idle_process
    sw $2, pcb_ear($1)

# Setup the $cctrl field
    sw $5, pcb_cctrl($1)

# Interrupt setup
#########################################	
	
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

# Restore registers
    lw $5, 2($sp)
    lw $2, 1($sp)
    lw $1, 0($sp)
    addui $sp, $sp, 3       # Destroy stack frame

    j load_context          # Begin first process



######################################################################
# PROCESSES MULTITASKED BY THE KERNEL
######################################################################	

serial_process:
    j serial_main
    
parallel_process:
    j parallel_main
    
gameSelect_process:
    j gameSelect_main

idle_process:
    sw $0, par_ctrl($0)     # Enable individual control of SSD segments
loop:
    addui $6, $0, 0
    lw $7, idle($6)
    sw $7, par_ulssd($0)

    addui $6, $6, 1
    lw $7, idle($6)
    sw $7, par_urssd($0)

    addui $6, $6, 1
    lw $7, idle($6)
    sw $7, par_llssd($0)

    addui $6, $6, 1
    lw $7, idle($6)
    sw $7, par_lrssd($0)

    j loop          # Infinite loop



######################################################################
# HANDLERS FOR EXCEPTIONS AND INTERRUPTS
######################################################################

handler:
    movsg $13, $estat       # The cause of the interrupt
    andi $13, $13, 0x40     # Check if the interrupt is because of the programmable timer (IRQ2)
    bnez $13, handle_irq2   # No other interrupt; the timer caused it
    lw $13, old_vector($0)  # Restore the old old_vector as the handler
    jr $13                  # This will stop counting

handle_irq2:
    sw $0, timer_iack($0)   # Acknowledge the IRQ2 interrupt

# Increment counter in serial task    
    lw $13, counter($0)     # Load counter from memory
    addi $13, $13, 1        # Increment counter by one
    sw $13, counter($0)     # Save new counter value

handle_time_slice:
    lw $13, time_slice($0)
    subui $13, $13, 1
    sw $13, time_slice($0)

    beqz $13, dispatcher
    rfe                 # If not, return from exception



######################################################################
# DISPATCHER AND SCHEDULER TO SAVE CONTEXT AND LOAD NEXT PROCESS
######################################################################

dispatcher:
save_context:

# Get the base address of the current PCB
    lw $13, current_process($0) 

# Save the registers
    sw $1, pcb_reg1($13)
    sw $2, pcb_reg2($13)
    sw $3, pcb_reg3($13)
    sw $4, pcb_reg4($13)
    sw $5, pcb_reg5($13)
    sw $6, pcb_reg6($13)
    sw $7, pcb_reg7($13)
    sw $8, pcb_reg8($13)
    sw $9, pcb_reg9($13)
    sw $10, pcb_reg10($13)
    sw $11, pcb_reg11($13)
    sw $12, pcb_reg12($13)
    sw $sp, pcb_sp($13)
    sw $ra, pcb_ra($13)

# Save old value of $13
    movsg $1, $ers                  # $1 is saved now so we can use it
    sw $1, pcb_reg13($13)
 
# Save $ear
    movsg $1, $ear
    sw $1, pcb_ear($13)
 
# Save $cctrl
    movsg $1, $cctrl
    sw $1, pcb_cctrl($13)

schedule:
    subui $sp, $sp, 1               # Create a small stack frame
    sw $2, 0($sp)                   # Save the value of only one register

    lw $13, current_process($0)     # Get current process
    sw $13, previous_process($0)    # Set as previous process
    lw $13, pcb_link($13)           # Get next process from pcb_link field
    sw $13, current_process($0)     # Set next process as current process

    la $2, gameSelect_pcb           # Get the address of the game select process
    sequ $13, $13, $2               # Check if it is the next process
    bnez $13, set_time_slice_games  # If next process is the game select, prioritise it

    lw $2, 0($sp)                   # Restore the register used
    addui $sp, $sp, 1               # Destroy the stack frame

set_time_slice_ports:   

# The next process is either the serial task or parallel task
    addui $13, $0, 1
    sw $13, time_slice($0)
    bnez $13, load_context

set_time_slice_games:   

# The next process is game select
    addui $13, $0, 4
    sw $13, time_slice($0)
    bnez $13, load_context

load_context:
    lw $13, current_process($0) # Get PCB of current process

# Get PCB value for $13 back into $ers
    lw $1, pcb_reg13($13)
    movgs $ers, $1

# Restore $ear
    lw $1, pcb_ear($13)
    movgs $ear, $1

# Restore $cctrls
    lw $1, pcb_cctrl($13)
    movgs $cctrl, $1

# Restore all other registers
    lw $1, pcb_reg1($13)
    lw $2, pcb_reg2($13)
    lw $3, pcb_reg3($13)
    lw $4, pcb_reg4($13)
    lw $5, pcb_reg5($13)
    lw $6, pcb_reg6($13)
    lw $7, pcb_reg7($13)
    lw $8, pcb_reg8($13)
    lw $9, pcb_reg9($13)
    lw $10, pcb_reg10($13)
    lw $11, pcb_reg11($13)
    lw $12, pcb_reg12($13)
    lw $sp, pcb_sp($13)
    lw $ra, pcb_ra($13)
    rfe                         # Return to new process

exit_process:
    subui $sp, $sp, 3           # Create stack frame

# Save some registers to use   
    sw $1, 0($sp)
    sw $2, 1($sp)
    sw $3, 2($sp)

# Manipulate linked list
    lw $2, current_process($0)  # Load process that is exiting
    lw $1, previous_process($0) # Load process that ran before this one

    sequ $3, $2, $1             # Check if this is the last task
    beqz $3, set_next       # If not, move to next processs

set_idle:
    la $3, idle_pcb             # All tasks have exited, execute idle process
    sw $3, pcb_link($1)         # Set idle process to be next
    j return

set_next:
    lw $2, pcb_link($2)         # Get the next process in the list
    sw $2, pcb_link($1)         # Set its next process to current's next process

return:

# Restore registers
    lw $3, 2($sp)
    lw $2, 1($sp)
    lw $1, 0($sp)
    addui $sp, $sp, 3           # Destroy stack frame
    jr $ra                      # Return


######################################################################
# ASSIGN SPACE FOR DIRECTIVES
######################################################################

.data
time_slice:         .word 1
idle:                                        
                    .word 0x76 #H
	                .word 0x79 #E
	                .word 0x38 #L
	                .word 0x38 #L

.bss
old_vector:         .word
current_process:    .word
previous_process:   .word

# Define stack spaces
#########################################	
    .space 200             # Stack label is below because stacks grow form the top of the stack
serial_stack:              # towards the lower addresses

	.space 200
parallel_stack:	

	.space 200
gameSelect_stack:	

    .space 200
idle_stack:

# Define PCB spaces
#########################################	
serial_pcb:
    .space 18

parallel_pcb:
	.space 18

gameSelect_pcb:
	.space 18

idle_pcb:
    .space 18