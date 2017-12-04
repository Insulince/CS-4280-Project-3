# COMPILER

To describe the details of this project, I will be using the following example:
       
Source:

    Var a.
    Begin
        Var a.
        a: 0;
        Check [a == 0] Begin
            Output a;
        End
        a: 0;
    End
    
    
Target:

    LOAD 0
    STORE I1
    BR L0
    STOP
    L1: NOOP
    LOAD 0
    STORE I1
    STOP
    L0: NOOP
    LOAD 0
    STORE I2
    LOAD I1
    SUB I2
    STORE I4
    MULT I4
    BRPOS L1
    LOAD I1
    STORE I3
    WRITE I3
    BR L1
    STOP
    I0 0
    I1 0
    I2 0
    I3 0
    I4 0
        
## RELATIONAL OPERATOR CHECKS:
All of these except for `==` were easy to check for. Each of the `<`, `<=`, `>`, `>=`, and `!=` mapped nicely to a different `BR` command. `==` gave me a bit of
trouble because I couldn't figure out how to check if two values were equal without some way to negate the `BRZERO` result, sort of like a `BRNOTZERO`. My solution
works like this:

Take the right side of the relation, subtract from the left side. This gives us the difference between the two values. If we are checking for things to be
equal, the difference would be zero, so `BRZERO`, right? Well the way that relational operators are used in loops and ifs, we always negate the meaning of
the RO because we only care if the condition is `FALSE`. So, we need to determine when it is false, which would mean the difference is *not* zero (hence the
`BRNOTZERO` I mentioned earlier). If I could guarantee the difference was always a positive number I would be golden because I would use a simple `BRPOS`, but
I can't guarantee that. For example, `Check [9 == 10] ...` would result in `9 - 10 = -1` which is *less* than 0, meaning `BRPOS` would give us the incorrect
result. A shallow attempt to fix this would be to simply multiply by `-1`, and this fixes the results where the left side is less than the right side, but it
*breaks* all of the results where the left side is greater than the right. Well after thinking about it I determined that there is a solution, and that is to
square the number and execute `BRPOS` on that. The reasoning is that squaring zero (meaning the two terms are equal) leads to zero still, but squaring any
other value positive or negative (meaning the two terms are *not* equal) would result in a positive value, thus `BRPOS` on that value. The exact code looks
like this (`IX` is a throw away variable):
    
        STORE IX    ]  --- Store the original value as IX.
        MULT IX     ]  --- Multiply the accumulator by IX (this is equivalent to IX * IX or IX^2).
        BRPOS       ]  --- Check if the result is positive (the only alternative is zero).

## IFs:
The key parts to notice here are the labeled segments. The program starts by doing the usual program commands. Once we reach the point where there should
be an if statement, we reach the code `BR LX`, where `X` is some integer. This is a label to a specific segment dedicated to processing the if statement. If you
examine the segment `LX`, it is composed of 4 parts:
    
1. Setting up the accumulator for evaluating the if statement.
2. Executing a BR statement (depending on the relational operator used) to check if the if statement is true.
    
    a. If it is false (meaning the condition in the if statement is true) we continue past the BR statement.
    
    b. If it is true (meaning the condition is false) we break to LY, where Y is an integer and Y != X.
3. Executing the body of the if statement.
4. Breaking back to the main program via BR LY (same Y as before).

So the way this looks in the segment:

    LOAD 0      -,
    STORE I2     |
    LOAD I1      | --- 1. We set up the accumulator to hold the result of the if statement.
    SUB I2       |
    STORE I4     |
    MULT I4     -'
    BRPOS L1    ]  --- 2. We break back to main if the if statement meets a specific condition leading to it being "false" (so we don't execute the body).
    LOAD I1     -,
    STORE I3     | --- 3. Execute the statement body. This is reached only if the condition is "true".
    WRITE I3    -'
    BR L1       ]  --- 4. Break back to main since we are finished executing the body of the if statement.
Now, following the original `BR LX` line is a `STOP`, which is included for safety against seg-faults, in reality it is never executed, but below it is the line
`LY: NOOP`. This line is where execution of main resumes.

## LOOPs:
For loops the process is practically identical, except for in part 4. of the segment specification I detailed earlier. Instead of breaking directly back to
main, we break back to the beginning of the loop where the accumulator is reloaded with the condition, and the condition is checked again. So instead of
`BR LY` at the end, it would be `BR LX`.

## SCOPING:
I used local scoping, meaning any time you enter a block any variables described in this block are scoped to it. This entails masking variables outside of the
current scope that share the same name as a variable in this scope, and also not redefining a variable in the same scope. All of this is handled at compile
time, but the effect it has on the target code is interesting. To allow for masking of values, I map each variable to a unique and unused throw-away variable.
So for example the global variable `a` in the above code would be mapped to `IX`. Now when we enter the first block and `a` is defined again, we map that `a` to a
new throw away variable `IY`. Both variables are technically 100% available to the target code at any point, but the compiler intentionally only grabs the latest
throw away variable it can find, so when doing `a: 0` in the first scope, it will pick the throw away variable `IY` to assign a value to, leaving `IX` as it was.
The way this functionality was implemented is through the `Scope` object, which is created at each `block`, and also one for the `global` scope. Each scope object
has a `vector` containing each variable encountered. It also has a `vector` that corresponds to the throw-away variable it maps to, so for this example on line 6 of
the provided code (in the body of the if statement) our scopes would look like (informal notation):

    Scope 0 (Global):
        variables: [a]
        throwAwayVariables: [IX]
    Scope 1 (Main Body):
        variables: [a]
        throwAwayVariables: [IY]
    Scope 2 (If Statement Body):
        variables: []
        throwAwayVariables: []
So when we execute `Output a` the compiler does the following:

    Is "a" in Scope 2? No, continue to the next scope down. Is "a" in Scope 1? Yes. What is it's throw away variable? "IY". Use that value.
Also, when exiting a scope, we `pop` the current top-most scope off of the scope `vector` maintained by the compiler (the reverse occurs when entering a new
scope).

## STORAGE ALLOCATION:
I opted to use global storage allocation because I didn't see much benefit from using the stack on tge virtual machine for how much work it would take to
implement it. I am sure it improves the efficiency of your storage management quite a bit, but I would rather focus on a solid implementation of the logic
and thorough testing instead. But anyway, the first thing you may notice on my storage allocation segment is that there are no variables referenced from the
code stored anywhere. Only throw-away variables. This is because of the way that I handle scoping, if you read the previous section you would understand that
every variable in every scope is mapped to a throw away variable. And because I need to use those to manage variable visibility between scopes, I never need to
use the original variable. Thus I only allocate storage for the throw away variables.

## REVIEW:
So let me repost the code with documentation now that we have covered all bases:

Source:
 
    Var a.
    Begin
        Var a.
        a: 0;
        Check [a == 0] Begin
            Output a;
        End
        a: 0;
    End

Target:

                   LOAD 0      -,--- Assign 0 to a in this scope.                                   -,
                   STORE I1    -'                                                                    |
            ,--    BR L0       ]--- Enter the if segment.                                            |
            |      STOP        ]--- Seg-fault protection. This is never executed.                    |--- MAIN program execution.
         ,--+->    L1: NOOP    ]--- Return label for resuming main program execution.                |
         |  |      LOAD 0      -,--- Assign 0 to a in this scope.                                    |
         |  |      STORE I1    -'                                                                    |
      ,--+--+--    STOP        ]--- END of the program is here. Proceed to storage allocation.      -'
      |  |  '->    L0: NOOP    ]--- Label for if statement.                                         -,
      |  |         LOAD 0      ]--- Load RHS of relation.                                            |
      |  |         STORE I2    ]--- Store to free up accumulator.                                    |
      |  |         LOAD I1     ]--- Load LHS of relation.                                            |
      |  |         SUB I2      ]--- Compute difference (LHS - RHS)                                   |
      |  |         STORE I4    ]--- Store to free up accumulator for squaring.                       |
      |  |         MULT I4     ]--- Square value of the difference.                                  |--- IF segment.
      |  |'----    BRPOS L1    ]--- IF its positive, then the relation is false, exit the if.        |
      |  |         LOAD I1     -,                                                                    |
      |  |         STORE I3     |--- Output value of a in this scope.                                |
      |  |         WRITE I3    -'                                                                    |
      |   '----    BR L1       ]--- Return to main program.                                          |
      |            STOP        ]--- End of segment. Also required for proper storage allocation.    -'
      '------->    I0 0        ]--- For the global a.                                               -,
                   I1 0        ]--- For the main body a.                                             |
                   I2 0        ]--- For the RHS of the relation.                                     |--- Allocate storage for all throw-away variables.
                   I3 0        ]--- For the calculation of the <expr> in the body of the if.         |
                   I4 0        ]--- For the difference of the terms in the relation.                -'