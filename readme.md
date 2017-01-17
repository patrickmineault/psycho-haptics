# Psychophysics for haptics

An haptics psychophysics experiment. We test how easy or hard it is to tell 
apart different haptic stimuli via a psychophysical experiment. ERM motors were
placed in the subject's palm in a cross pattern, and the subject had to say 
whether the two stimuli presented in quick succession were the same or 
different. 

The .ino file is an Arduino sketch that runs a same/different psychophysics 
experiment. It expects ERM motors on the specified pins of an Arduino Uno. 
A setting controls whether the signal is relayed by a pulse in one motor, or by 
a quick succession of pulses in two distinct motors. Various parameters of the 
psychometric curve are tested during the experiment - for imstance, the strength
of the stimulation - and the experiment finishes whenever all the 
combinations of the parameters have been presented to the user (fixed randomized
factorial design). The data from the experiment are collected using the serial
window of the Arduino editor; the results are printed as they are gathered in a 
CSV format, which is then copied by the experimenter to a csv file.

The *.csv files are files that were gathered using the sketch, under different
conditions.

The .R files analyze the experiment and spits out some good looking plots. 
