# NepControl for the RF Amplifier Neptune 50W with automatic band switch

add an ESP32 to control the amp better and fix the issue "PA switched OFF because faulty frequency detection" in mode SSB.

The issue isn't a really issue (PA switched immediately back to OFF/bypass if using mode SSB) - is a decision of the manufacturers device firmware design to protect the LPF against wrong band selection.
For me this firmware design in it's form is not acceptable, because it continually breaks the normal use of this PA in mode SSB and in my case if running FT8 too.

My solution to bring back the PA to a usable condition: I take the control over the main control signals like BIAS on/off and RX/TX relais with my own ESP32 controller and changed PA switch logic, which fix this "issue" completly. The "issue" is gone after my mod.
Additional I replace with this modification the automatic band detection through band select with bandvoltage output from the Hermes-Lite 2.

My mod is only allowed under this condition, because my mod remove the protection, which is causing the issue with SSB. A wrong band selection can damage the LPF capacitors !
<hr>
If you want to use my mod, you confirm the following:<BR>
No guarantee, no warranty !<BR>
You do all at your own risk !<BR>
You know, what you do and why !<BR>
Only for use in amateur radio, because you need to be a licensed hamradio operator !
