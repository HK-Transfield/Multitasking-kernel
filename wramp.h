/*

Note for students: This licence warning is big and scary, but
it basically boils down to the following:

- Feel free to use and take copies of this code, as long as the licence
    notice stays intact. Although you might find it more convenient
    to link against it from the shared folder, in case we update the
    file in the future.
- If you take a copy of this file and publish your code, you need to keep
    the licence intact. Of course, it's probably not a good idea to do
    that, since you get charged equally if someone uses your code and you
    get caught for plagiarism! You're also not allowed to make money from
    code you write that depends on this file.

You can find the full source for WRAMPmon at the following address:
https://github.com/wandwramp/WRAMPmon

########################################################################
# This file is part of WRAMPmon, the WRAMP monitor programe.
#
# Copyright (C) 2019 The University of Waikato, Hamilton, New Zealand.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
########################################################################
*/
/**
 * WRAMP Serial Ports
 **/
typedef volatile struct
{
	int Tx;
	int Rx;
	int Ctrl;
	int Stat;
	int Iack;
} WrampSp_t;

// Bit flag defines
#define WRAMP_SP_TDS		0x002
#define WRAMP_SP_RDR		0x001
#define WRAMP_SP_ERR_IE		0x400
#define WRAMP_SP_TDS_IE		0x200
#define WRAMP_SP_RDR_IE		0x100

/**
 * WRAMP Timer
 **/
typedef volatile struct
{
	int Ctrl;
	int Load;
	int Count;
	int Iack;
} WrampTimer_t;

#define WRAMP_TIMER_RESTART		0x2
#define WRAMP_TIMER_ENABLE		0x1
 
/**
 * WRAMP Parallel Port
 **/
typedef volatile struct
{
	int Switches;
	int Buttons;
	int LeftSSD;
	int RightSSD;
	int Ctrl;
	int Iack;
	int UpperLeftSSD;
	int UpperRightSSD;
	int LowerLeftSSD;
	int LowerRightSSD;
	int LED;
} WrampParallel_t;

#define WRAMP_PAR_IE			0x2
#define WRAMP_PAR_HEX_DECODE	0x1
 
/**
 * WRAMP User Interrupt Button
 **/
typedef volatile struct
{
	int Iack;
} WrampUserInt_t;


/**
 * Declarations
 **/
#define WrampSp1 		((WrampSp_t*)0x70000)
#define WrampSp2 		((WrampSp_t*)0x71000)
#define WrampTimer  	((WrampTimer_t*)0x72000)
#define WrampParallel	((WrampParallel_t*)0x73000)
#define WrampUserInt	((WrampUserInt_t*)0x7f000)
