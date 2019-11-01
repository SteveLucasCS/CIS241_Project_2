## Event-Driven Simulation

This project simulates a **single-server queue** at local bank.

The model assumes the following:
- There is a **single line of customers**
- There are **multiple teller stations**
- Each teller serves **one customer at a time**
- Customers **join the queue immediately** upon entering the bank
- Customers in queue **move to available tellers immediately**

---
### Reference Data
This program uses a .dat file as reference, allowing it to be module and applied to other business models
in the future.
Empirical data was obtained through observations over a **three month period**.
The data shows the **number of customers** who **arrived during one-minute intervals** throughout the day.
---
### Functionality
In the simulation program, the **number of arriving customers** can be calculated by using a **randomly
generated number from 1 to 100**. The above table identifies five ranges of numbers from 1 to 100,
based on the percentages of one-minute intervals which had 0, 1, 2, 3, or 4 customers. If the random
number is 40, for example, it falls into the range 36 to 60 so the number of arriving customers is 2;
if the random number is 90, then the number of arriving customers is 4. This method simulates the rate at
which customers arrive at the bank.