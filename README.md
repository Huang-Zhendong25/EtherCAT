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
## Problem
When using TwinCAT3 as an EtherCAT master, connecting a slave and entering the Run Mode (DC synchronization), ***synchronization errors*** are often reported. At the same time, TwinCAT displays that the slave has reverted from the OP state to the SAFEOP state, and immediately returns to the OP state. This can cause the slave to temporarily freeze during opeartion and sometimes even stop randomly in the SAFEOP state. Additionally, when scanning the slave and entering the Config Mode (or reverted from Run mode and then reset the slave to entering Config Mode or Free-Run Mode), it also stops randomly in the SAFEOP state, and it's necessary to try to re-scan to enter the OP state.<br>
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/Sychronization%20error.png?raw=true)
## Components of Slave
1. Physical Layer and Data Link Layer: FRJ45056 network port (with built-in isolation transformer), KS8721BLI-TR PHY chip, ET1100 EtherCAT Slave Controller.
2. Application Layer: STM32F103CBT6.
3. Using SPI as PDI, and using 3 external interrupt pins (EXT_IRQ, SYNC0, SYNC1) of STM32 to receive the interrupt signals (falling edge active) from ET1100.
## Theoretical Analysis
***AL Status Code*** is provided by TwinCAT, and it's a diagnostic information at the application layer, so it can naturally be found in the EtherCAT Slave Code (ESC). Therefore, the hexadecimal code can be found in the ESC (ecatslv.h specifically) according to AL Staus Code to obtain a simple explanation. Additionally, by using certain conditional judgements in ecatslv.c and other files, the conditions that meet this hex code can be checked, thereby inferring the possible causes.<br>
On this basis, a counter ***sSyncManOutPar.u16SmEventMissedCounter*** can be found in ecatslv.c, and TwinCAT will report an error as long as this counter's value exceed sErrorSettings.u16SyncErrorCounterLimit. At a glance, the value of this counter will decrease 1 in function PDI_Isr(), and increase 3 in function Sync0_Isr(). <br>
In the manual *Application Note ET9300 (EtherCAT Slave Stack Code)* provided by Beckhoff, there's such a reminder for the aforementioned situation, which might be helpful: *If the SM2-event is too late to complete the CalcAndCopy before the Sync0-Event occurs, the "SmEventMissed-Counter" is incremented and if the SmEventMissedLimit is exceeded the slave goes to SafeOpErr.*<br>
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/SM_Sync0_Synchronization.png?raw=true)
<br>However, these analyses only allow us to conclude that the cause of the Synchronization Error is most likely related to the SM2 Event and the SYNC0 Event, and it's possible that the SM2 Event had not been completed when the SYNC0 Event arrived. From the result, it can be seen that this theory is indeed reasonable, but it is not sufficient to solve the problem.<br>
## Practical Analysis
When new process data arrives at the slave, it will trigger the SM2 Event, which will cause ET1100 to send an EXT_IRQ interrupt signal to STM32, and the STM32 will then read the registers of ET1100 in the corresponding interrupt handling function to obtain the process data (the specific process can also be found in chapter *Synchroniation* of *Application Note ET9300*). After a certain period of time, the ET1100 will send a SYNC0 synchronization interrupt signal to the STM32, and in the corresponding interrupt handling function, the STM32 will complete the application layer operation.<br>
Using a logic analyzer to capture signals EXT_IRQ, SYNC0 and SPI, it was discovered that the STM32 would repeatedly read the registers of ET1100 and decode the SPI protocol, this repetitive process is <br>`2202ff 220aff 2212ff 221aff` <br>`4062ff 406aff 4072ff 407aff` <br>`0002ff 0002ff`<br>
It should be noted that the function used for reading in STM32 will continuously read 4 registers. This is accomplished by a while loop. At the beginning of the loop, the global interrupts are disabled, and they will be re-enabled after reading 1 byte.Futhermore, the interrupt handling function triggered by EXT_IRQ signal will read specific registers, this process is <br>`00002ff 88020000000000000000000000ff 0002ff`<br>During this process, the process data will also be read. Therefore, in the captured normal waveforms, it can be observed that the cyclic reading process is randomly interrupted by the EXT_IRQ signal, even when reading 4 consecutive registers, a long string of process data is read after the interruption.<br>
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/EXT_IRQ.png?raw=true)
Therefore, it can be concluded that as long as the EXT_IRQ operation is executed correctly every time in a normal SYNC0 state, there will be no synchronization error problem. This indeed aligns with the *Application Note ET9300*. However, ***the actual situation is that sometimes, even though the EXT_IRQ signal reaches the STM32 normally and the STM32 can read the low level of the pin, for some unknown reasons, the STM32 does not execute the corresponding interrupt handling function, which directly leads to the synchronization error***.<br>
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/Error%20EXT_IRQ.png?raw=true)
A piece of code was added to the while loop of the SPI read function to send the register values related to the STM32 interrupt. Thus, even if the EXT_IRQ signal didn't trigger the interrupt operation, the SPI protocol could be decoded in the logic analyzer to check whether the current interrupt was suspended or blocked, etc. It was proven that when an error occurred, the interrupt was not even suspended at this time, which was equivalent to no interrupt signal. At this point, the ET1100 registers are still being read in the loop, and no interrupt operation that must be executed is performed. Only after executing the interrupt operation and reading those specific registers, will ET1100 reset the interrupt signal to high. Therefore, EXT_IRQ was not raised, precisely speaking, the error occurred after 3 SYNC0s, and then EXT_IRQ was forcibly raised, which is also the reason why the slave can return to the OP state again.<br>
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/Error%20EXT_IRQ2.png?raw=true)
## Solution
Since the synchronization error occurred due to the absence of the execution interruption function, a very straightforward approach would be to ***manually compensate***. When the EXT_IRQ signal arrives, a judgement should be made: If the interrupt handling has been executed corretly, no action should be taken; If not, then manually perform the operations that the interrupt handling function executes (that is reading the specific registers mentioned earlier). In simple terms, at the end of a while loop in the SPI read function (at this point, the global interrupt has just been re-enabled), if the current register address to be read does not meet the requirement (it indicates that the interrupt has not been responded to), then a ***soft interrupt*** is triggerd. In the soft interrupt, the compensation operation is executed to read the specific registers. To prevent being interrupted, in the soft interrupt, the global interrupt should be diabled and re-enabled natually.<br>
## Result
During the operation of slaves, synchronization errors rarely occur. At this time, if the slave station is disconnected and then re-powered on, it's only necessary to reload the slaves instead of scanning again, and it can smoothly enter the OP state. Moreover, in the Free-Run mode, process data can be transmitted freely.<br>
However, it might be necessary to optimize the judgement logic and the soft interrupt handling function, because based on the captured waveforms, it can be observed that during actual operation, there are occasional occurrences of 2 interrupt handling function operations following EXT_IRQ. According to the frequency of this occurrence, it's estimated that 2 compensations were carried out consecutively. This is because `88020000000000000000000000ff` was present both before and after `00002ff`. The source coded provided by this repository doesn't include a second judgement.<br>
## Question
What exactly causes EXT_IRQ to sometimes fail to be responded to? Is this a feature of the STM32 microcontroller? Or is it the feature of the F103 series? if it is said that the interruption was not responded to, then why didn't the SYNC0 signal have an error? Or, to put it another way, the previous analysis was incorrect, and the real reason is not like this?
![image](https://github.com/Huang-Zhendong25/Huang-Zhendong25/blob/main/pictures/Read_twice.png?raw=true)
## Acknowledgements
I would like to express my gratitude to my senior classmate YXY, I am very grateful for his assistance and guidance.
