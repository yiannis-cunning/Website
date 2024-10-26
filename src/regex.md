# Regex project

[Github repo](https://github.com/yiannis-cunning/regex), [ascii table](https://www.asciitable.com/), [Regex general rules](https://support.dataset.com/hc/en-us/articles/360047505053-DataSet-Regex)

## Mini-regex specification

For use on ASCII strings. Supported special characters:

| Character | Meaning|
| --------- | ------ |
| |         | OR two possible matches |
| *         | Repeat the previous group 0 or more times |
| +         | Repeat the previous group 1 or more times |
| ?         | Match to previous group or skip |
| ( )       | Start grouping/end grouping |
| []        | Match to a caracter set |

#### Not supported right now:
* Macros of common character sets (ie: \w \d ... ext)
* Repeat grouping N times operation {N}
* Inverse of character set [^x]
* Match to start or end of string ($ / ^)

#### Character sets:
Will take characters inside as literal. If there is a dash (-) then the range between the two ASCII codes will be taken.

## Usage
```c
    regex_t x = new regex_t(char *s);
    bool a = x.match("Testinput");
    if(a){
        printf("Input matched regex!\n");
    } else{
        printf("Regex did not match!\n");
    }

```


## Implementation
![img](imgs/image.jpg)
### (1) Tokenization


All inputs will be turned in to uniform tokens.
Their type will be one of the following:
* STRING - string of consecutive normal characters
* SPCL - special characters that operate on a group - * / + / ?
* CCL/CCR/SQR/SQL - left and right for both types of bracket
* NULL - end of the string


### (2) Create Parse Tree

The tokens will be parsed according to the following rules:
* S -> Regex
* Regex -> expr <|> Regex
* Regex -> expr
* expr -> grp spcl
* expr -> expr expr
* grp -> <(> Regex <)>
* grp -> <[> string <]>
* grp -> string

Higher up rules have a higher precidence. Using the grammar a parse table can be constructed and a parse tree can be made for the regex input. This code can be generated using a tool such as ANTLR4.

In this code LR(1) parsing is used. Below can be seen the complete parse state diagram:

### (3) Convert to AST
Using the parse tree, a abstract syntax tree can be made to put the structure of the regext in a more usable form. This is done after the parse tree is completley made.

### (4) Create NFA
Use the AST to create a NFA graph.

### (5) DFA Construction
Once the NFA is made it can be converted into a DFA.


### (6) Regex traversal
Now that the DFA is made it is very simple to check if a new input string will match to the given Regex. The DFA can be looped to and if the input gets completed and you are on a finished state the string is accepted.
