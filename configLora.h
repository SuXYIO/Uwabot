#pragma once
#include "pins.h"
#include <Arduino.h>

/*
Functions for configuring Lora module

Before me, the team used AS_DS.exe to configure Lora via a computer GUI,
but although easier, I think it's quite time wasting, since you have to connect the cables every time.
Since there are write commands in the docs, I implemented these, which lets arduino configure it.
Dunno if the exchange in time and performance (size of binary code) is sensible,
I'm new to embedded systems after all.
So I won't be really surprised if one day, after adding a bunch of functionalities,
we'll have to delete this due to waste of memory and storage.

All code here is oriented for A39C-T400A22D1a lora module
*/

void loraConfig();
