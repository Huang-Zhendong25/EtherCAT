# EtherCAT "Synchronization Error (AL Status Code: 0x001a)" 🤔

<!--
**Huang-Zhendong25/Huang-Zhendong25** is a ✨ _special_ ✨ repository because its `README.md` (this file) appears on your GitHub profile.

Here are some ideas to get you started:

- 🔭 I’m currently working on ...
- 🌱 I’m currently learning ...
- 👯 I’m looking to collaborate on ...
- 🤔 I’m looking for help with ...
- 💬 Ask me about ...
- 📫 How to reach me: ...
- 😄 Pronouns: ...
- ⚡ Fun fact: ...
-->
## Problem:
When using TwinCAT3 as an EtherCAT master, connecting a slave and entering the Run Mode (DC synchronization), ***synchronization errors*** are often reported. At the same time, TwinCAT displays that the slave has reverted from the OP state to the SAFEOP state, and immediately returns to the OP state。 This can cause the slave to temporarily freeze during opeartion and sometimes even stop randomly in the SAFEOP state. Additionally, when scanning the slave and entering the Config Mode (or reverted from Run mode and then reset the slave to entering Config Mode or Free-Run Mode), it also stops randomly in the SAFEOP state, and it's necessary to try to re-scan to enter the OP state.
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/Sychronization%20error.png?raw=true)
## Components of Slave:
1. Physical Layer and Data Link Layer: FRJ45056 network port (with built-in isolation transformer), KS8721BLI-TR PHY chip, ET1100 EtherCAT Slave Controller.
2. Application Layer: STM32F103CBT6.
3. Using SPI as PDI, and using 3 external interrupt pins (EXT_IRQ, SYNC0, SYNC1) of STM32 to receive the interrupt signals (falling edge active) from ET1100.
## Theoretical Analysis
***AL Status Code*** is provided by TwinCAT, and it's a diagnostic information at the application layer, so it can naturally be found in the EtherCAT Slave Code (ESC). Therefore, the hexadecimal code can be found in the ESC (ecatslv.h specifically) according to AL Staus Code to obtain a simple explanation. Additionally, by using certain conditional judgements in ecatslv.c and other files, the conditions that meet this hex code can be checked, thereby inferring the possible causes.
On this basis, a counter ***sSyncManOutPar.u16SmEventMissedCounter*** can be found in ecatslv.c, and TwinCAT will report an error as long as this counter's value exceed sErrorSettings.u16SyncErrorCounterLimit. At a glance, the value of this counter will decrease 1 in function PDI_Isr(), and increase 3 in function Sync0_Isr(). 
In the manual *Application Note ET9300 (EtherCAT Slave Stack Code)* provided by Beckhoff, there's such a reminder for the aforementioned situation, which might be helpful: *If the SM2-event is too late to complete the CalcAndCopy before the Sync0-Event occurs, the "SmEventMissed-Counter" is incremented and if the SmEventMissedLimit is exceeded the slave goes to SafeOpErr.*
However, these analyses only allow us to conclude that the cause of the Synchronization Error is most likely related to the SM2 Event and the SYNC0 Event, and it's possible that the SM2 Event had not been completed when the SYNC0 Event arrived. From the result, it can be seen that this theory is indeed reasonable, but it is not sufficient to solve the problem.
## Practical Analysis
When new process data arrives at the slave, it will trigger the SM2 Event, which will cause ET1100 to send an EXT_IRQ interrupt signal to STM32, and the STM32 will then read the registers of ET1100 in the corresponding interrupt handling function to obtain the process data (the specific process can also be found in chapter *Synchroniation* of *Application Note ET9300*). After a certain period of time, the ET1100 will send a SYNC0 synchronization interrupt signal to the STM32, and in the corresponding interrupt handling function, the STM32 will complete the application layer operation.
Using a logic analyzer to capture signals EXT_IRQ, SYNC0 and SPI, it was discovered that the STM32 would repeatedly read the registers of ET1100 and decode the SPI protocol, this repetitive process is <br>`2202ff 220aff 2212ff 221aff` <br>`4062ff 406aff 4072ff 407aff` <br>`0002ff 0002ff`<br>
It should be noted that the function used for reading in STM32 will continuously read 4 registers. This is accomplished by a while loop. At the beginning of the loop, the global interrupts are disabled, and they will be re-enabled after reading 1 byte.Futhermore, the interrupt handling function triggered by EXT_IRQ signal will read specific registers, this process is <br>`00002ff 88020000000000000000000000ff 0002ff`<br>During this process, the process data will also be read. Therefore, in the captured normal waveforms, it can be observed that the cyclic reading process is randomly interrupted by the EXT_IRQ signal, even when reading 4 consecutive registers, a long string of process data is read after the interruption.
Therefore, it can be concluded that as long as the EXT_IRQ operation is executed correctly every time in a normal SYNC0 state, there will be no synchronization error problem. This indeed aligns with the *Application Note ET9300*. However, the actual situation is that sometimes, even thougn the EXT_IRQ signal reaches the STM32 normally and the STM32 can read the low level
