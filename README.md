# Expression Evaluator Calculator Using Shunting Yard Algorithm
 
* There are 6 operator types (DIV,MUL,SUB,UNARY_SUB,ADD,PERCENT)
 ```
struct token {
    enum type t;
    union {
        int precedence;
        double operand;
    } fields;
};
#define PLUS 10 /* + */
#define SUB 10  /* - */
#define MUL 11 /* * */
#define DIV 11 /* / */
#define PER 11 /* p */
#define UNARY  12 /* unary - */
```
* Operands are only double values, highest precedence operator is UNARY(unary_sub)
* Numbers collected as double value in C
* A p B is doing A * (B/100)
* Program has a one Text Box and Button, computed result printed in MessageBoxA function
* Does not support open '(' and closed ')' parentheses
  
``` Requires MSVC C Desktop Development Toolset ```
 <table border="0">
  <tr> 
<td> <img width="386" height="265" alt="image" src="https://github.com/user-attachments/assets/35788dc7-207f-4944-a597-fcc64cea0831" /> </td>
<td> <img width="408" height="266" alt="image" src="https://github.com/user-attachments/assets/c628a2fd-d321-45b2-8a80-a007d9f37e19" />  </td>
  </tr>
</table>

 <table border="0">
  <tr>
 <td> <img width="414" height="264" alt="image" src="https://github.com/user-attachments/assets/f66fb811-493d-445a-b178-0c52af094eea" /> </td>
  <td>  <img width="414" height="279" alt="image" src="https://github.com/user-attachments/assets/0e7a6e12-0be7-4de1-84b9-96517353891a" /> </td>
  </tr>
</table>


