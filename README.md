# PI9000

PI9000 is a machine that inspect the bonding (Flexible Printed Circuit on Flexible glass panel) result of previous machine.

Below is the operatin process:
1. InPickArm will pick up the panel from previous machine ,put it on WH1/WH2 and inspect the position of the panel on WH1/WH2.
2. WH1/WH2 will move to InspOpt accordingly to inspect the bonding results.
3. If the bonding results are good, OutPickArm will pick up the panel and pass it to the next machine, otherwise it will be thrown to the RejectBelt.
