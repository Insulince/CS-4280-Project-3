LOCAL scoping.
GLOBAL storage allocation.

Notes:

The way I implemented loops and ifs is by using labels. Here is a simple example:

Source:
    Var a.
    Begin
        a: 1;
        Check [a != 0] Begin
            Output a;
        End
        a: 0;
    End

Target:
    LOAD 1
    STORE I0
    BR L0
    STOP
    L1: NOOP
    LOAD 0
    STORE I0
    STOP
    L0: NOOP
    LOAD 0
    STORE I1
    LOAD I0
    SUB I1
    BRZERO L1
    LOAD I0
    STORE I2
    WRITE I2
    BR L1
    STOP
    I0 0
    I1 0
    I2 0

So, the key parts to notice here are the labeled segments. The program starts by doing the usual program commands. Once we reach the point where there should
be an if statement, we reach the code "BR LX", where X is some integer. This is a label to a specific segment dedicated to processing the if statement. If you
examine the segment "LX", it is composed of 4 parts:
    1. Setting up the accumulator for evaluating the if statement.
    2. Executing a BR statement (depending on the relational operator used) to check if the if statement is true.
        a. If it is false (meaning the condition in the if statement is true) we continue past the BR statement.
        b. If it is true (meaning the condition is false) we break to LY, where Y is an integer and Y != X.
    3. Executing the body of the if statement.
    4. Breaking back to the main program via BR LY (same Y as before).
So the way this looks in the segment:
    LOAD 0      -,
    STORE I1     |--- 1. We set up the accumulator to hold the result of the if statement.
    LOAD I0      |
    SUB I1      -'
    BRZERO L1   ] --- 2. We break back to main if the if statement meets a specific condition leading to it being "false" (so we don't execute the body).
    LOAD I0     -,
    STORE I2     |--- 3. Execute the statement body. This is reached only if the if statement is "true".
    WRITE I2    -'
    BR L1       ] --- 4. Break back to main since we are finished executing the body of the statement.
Now, following the original BR LX line is a "STOP", which is included for safety against seg-faults, in reality it is never executed, but below it is the line
"LY: NOOP". This line is where execution of main resumes. In our case it is followed by a simple "STOP", but that is only because we did not include any other
commands in the original source after the if statement. If we had, you would see their generated code here, which could include more if statement targets.
So for loops. the process is practically identical, except for in part 4. of the segment specification I detailed earlier. Instead of breaking directly back
to main, we break back to the beginning of the loop where the accumulator is reloaded with the condition, and the condition is checked again. So instead of
"BR LY" at the end, it would be "BR LX".