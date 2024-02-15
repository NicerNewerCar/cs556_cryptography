# Assignment 1 - Lombardi

## Plain text message:

```
WELCOME TO CRYPTOGRAPHY. THIS IS OUR FIRST HOMEWORK IN THIS COURSE.
FOR THIS ASSIGNMENT PLEASE SUBMIT THE PLAINTEXT OF THIS ENCRYPTION
AS AN EMAIL ALONG WITH A SHORT FILE DESCRIBING THE MONOALPHABETIC
CIPHERS USED IN THE ENCRYPTION. ALSO DESCRIBE HOW YOU CRACKED THIS
ASSIGNMENT. THIS ASSIGNMENT IS DUE IN ONE MONTH STARTING FROM TODAY.
```
## Ciphers
### Cipher 1:
```
{'M': 'W',
 'W': 'C',
 'Q': 'E',
 'V': 'P',
 'N': 'G',
 'G': 'S',
 'P': 'A',
 'C': 'I',
 'T': 'U',
 'E': 'N',
 'O': 'T',
 'K': 'D',
 'U': 'R',
 'X': 'M',
 'J': 'O',
 'R': 'K',
 'Z': 'F',
 'F': 'H',
 'B': 'L',
 'D': 'B'}
```

### Cipher 2:
```
{'A': 'E',
 'X': 'O',
 'L': 'T',
 'T': 'R',
 'J': 'H',
 'H': 'I',
 'S': 'A',
 'R': 'S',
 'U': 'N',
 'D': 'M',
 'B': 'L',
 'E': 'X',
 'F': 'G',
 'Z': 'Y',
 'C': 'U',
 'K': 'F',
 'G': 'C',
 'P': 'P',
 'Q': 'D'}
```

### Cipher3:
```
{'B': 'L',
 'W': 'M',
 'G': 'O',
 'H': 'Y',
 'Q': 'A',
 'J': 'N',
 'C': 'I',
 'R': 'T',
 'X': 'H',
 'V': 'D',
 'N': 'E',
 'M': 'F',
 'E': 'B',
 'Z': 'R',
 'A': 'G',
 'P': 'C',
 'T': 'P',
 'D': 'S',
 'S': 'K'}
```

## My Method

After our discusion I knew that the first three words were `WELCOME TO CRYPTOGRAPHY`. I used this knowledge to build up three dictionaries in python containing the current mappings.
I then decrypted the message (as much as I could) using the python function below. I then proceeded to start guessing at what I thought the words were and then adding those letters to 
the dictionaries and re-decrypting. I repeated this process (changing some mappings up as I went) until the message was completely decrypted.
```python
from termcolor import colored
def dec(cipher, map1, map2, map3):
    i = 0
    for c in cipher:
        if c.isalpha():
            if i % 3 == 0 and c in map1.keys():  # Map1
                print(colored(map1[c], "red"), end="")
            elif i % 3 == 1 and c in map2.keys():  # Map2
                print(colored(map2[c], "green"), end="")
            elif i % 3 == 2 and c in map3.keys():  # Map3
                print(colored(map3[c], "blue"), end="")
            else:
                print(c, end="")
            i += 1
        else:
            print(c, end="")
```
