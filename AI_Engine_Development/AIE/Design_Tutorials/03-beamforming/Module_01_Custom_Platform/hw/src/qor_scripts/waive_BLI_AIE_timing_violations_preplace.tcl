

#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

# Last update; 2019/09/06
# Last change: checking on AIE_PL BEL location to make sure there is a BLI site available
#
# Last update; 2019/11/20
# Last change: adding support for 128b interfaces
#

set debug 0
foreach aiePL [get_cells -quiet -hier -filter "REF_NAME=~AIE_PL_* && PRIMITIVE_LEVEL!=MACRO"] {
  set loc [get_property LOC $aiePL]
  if {$loc == ""} { if {$debug} { puts "D - Missing LOC - $aiePL" }; continue } ;# Unplace AIE_PL cell => unsafe to waive timing
  set bel [get_property BEL $aiePL]
  if {$bel == "AIE_PL.AIE_PL_S_AXIS_3"} { if {$debug} { puts "D - BEL 3 - $aiePL" }; continue } ;# No BLI register site available
  if {$bel == "AIE_PL.AIE_PL_S_AXIS_7"} { if {$debug} { puts "D - BEL 7 - $aiePL" }; continue } ;# No BLI register site available
  foreach dir {IN OUT} {
    set bliFD [get_cells -quiet -filter "REF_NAME=~FD* && BLI==TRUE" -of [get_pins -leaf -of [get_nets -filter "FLAT_PIN_COUNT==2" -of [get_pins -filter "!IS_CLOCK && DIRECTION==$dir" -of $aiePL]]]]
    if {$bliFD == {}} { if {$debug} { puts "D - no BLI FD - $dir - $aiePL" }; continue }
    set refName [get_property REF_NAME $aiePL]
    set locBel "$loc/[regsub {.*\.} $bel {}]"
    if {$dir == "IN"} {
      puts [format "INFO - Adding False Path waiver from %2s BLI registers to   $refName ($locBel) - $aiePL" [llength $bliFD]]
      set_false_path -from $bliFD -to $aiePL
    } else {
      puts [format "INFO - Adding False Path waiver to   %2s BLI registers from $refName ($locBel) - $aiePL" [llength $bliFD]]
      set_false_path -from $aiePL -to $bliFD
    }
  }
}

