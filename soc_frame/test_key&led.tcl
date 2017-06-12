# TCL that reads from a pushbutton and writes to LEDS (via Avalon MM)
# First while loop writes to the LEDs where the LED pio is at address 0x180 (Qsys environment)
# This section  was only used for testing that the LEDS were writing correctly
# Second while loop reads and writes to the register of LEDS and thus the LEDs, depending on the pushbutton 
# The addresses after the $jtag_master need to match the base env in the Qsys 
# When using this code on other boards, what needs to be changed is
#	1) Base addresses of the LEDs
#	2) Base addresses of the PIOs
#	3) Verify that LEDS and PIOs are on the same master (eg in Qsys, the same memory master)
#	If this is not the case, the memory master needs to be changed
# The System Console test Message will show that the test is done, but the push buttons can be still pushed to change the LEDs
# as long as the board is not reprogrammed 


# Set the values to write to the LED pio.
#
set led_vals { 1 2 4 8 16 32 64 128 256 512 256 128 64 32 16 8 4 2 1}
# 
set AvailableServices [get_service_types]

#sets the Service Path (needed for System Console)

set jtag_master [lindex [get_service_paths master] 0]

#opens the service master (needed for system console)

open_service master $jtag_master

#sets up loop value
set loopcount 0x00
#writes to LEDs going through the above values
#used for testing, and thus the loopcount is very low
#LED base address is 0x0001_0040 in Qsys, and thus set to 0x1_0040 in code

#while {$loopcount < 19} {
#	foreach val $led_vals {
#	master_write_16 $jtag_master 0x10040 $val
	# send_message info $loopcount 
#	after 1000 
#	incr loopcount
#	}
#   }

#sets up variables

set lastSwitch 0x00
set CurSwitch 0x00
#note variable of loopcount1 (not variable of loopcount from above)
set loopcount1 0x00

#loop to read
#reads from PIO (base address in Qsys is 0x1_00c0), and writes to LED (base address in Qsys is 0x1_0040)


while {$loopcount1 < 100} {

	# set the value to read from PIO starting from the base address and going up by 2 bytes

	set CurSwitchs  [master_read_8 $jtag_master 0x100c0 0x2]  
	send_message info $CurSwitchs
 
	# set the setting of CurSwitch to be the first value in the index (eg 0 in the lindex) 
	set CurSwitch [lindex $CurSwitchs 0 ]	

	
	#compare the CurSwitch to be value (lastSwitch) and depending on the result
	#write either value
 
	if { $lastSwitch == $CurSwitch} {
		# master_write_8 $jtag_master 0x1_0040 7
		 master_write_8 $jtag_master 0x10040 $CurSwitch  
		# 7 in binary is 0111
		# send_message info "write the value of 7 (in binary)"
		# send_message info "=CurSwitch" 
		# send_message info $CurSwitch  		
			
	}
	if { $lastSwitch != $CurSwitch} {
		# master_write_8 $jtag_master 0x1_0040 8
		 master_write_8 $jtag_master 0x10040 $CurSwitch  
		# send_message info "write the value of 1 (in binary) "	
		# send_message info "lastSwitch" 
		# send_message info $lastSwitch  
		 send_message info "CurSwitch" 
		 send_message info $CurSwitch  		
	}

	set lastSwitch $CurSwitch 
	incr loopcount1
  	 
}

#read the 2 byte at 0x1_00c0

master_read_memory $jtag_master 0x100c0 2

#print out a message that the test is done
send_message info "System Console test done"

#closes service master (needed for system console)

close_service master $jtag_master

