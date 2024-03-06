/*
MIT License

Copyright (c) 2024 Esc

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

int findLetterCount(char value) // 0 Indexed
{
  String letters[26] =
  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

  int count = 0;
  for (int i = 0; i < 26; i++)
  {
    if (String(value) == letters[i])
    {
      i = 26;
    }
    count = count + 1;
  }
  return count;
}

void loop() {
  // Creating a 2d array for morse
  // Doesnt contain numbers
  // There are objectively better ways to do this but this is funcional.
  // https://commons.wikimedia.org/wiki/File:International_Morse_Code.svg
  // Rhey T. Snodgrass & Victor F. Camp, 1922, Public domain, via Wikimedia Commons
  String lookup[26] =
  {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
  "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

  const int unit = 1000;
  const int dot_units = 1;
  const int dash_units = 3;
  const int letter_part_units = 1;
  const int letter_space_units = 3;

  // Main Code

  String name = "William";
  int name_len = name.length();

  for(int i = 0; i < name_len; i++)
  {
    char current_char = name[i];
    int index = findLetterCount(current_char);
    String morse = lookup[index];
    
    for (int j = 0; j < morse.length(); j++)
    {
      char current_morse = morse[j];
      if (String(current_morse) = ".")
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(unit * dot_units);
        digitalWrite(LED_BUILTIN, LOW);
        delay(unit * letter_part_units);
      }
      else if (String(current_morse) = "-")
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(unit * dash_units);
        digitalWrite(LED_BUILTIN, LOW);
        delay(unit * letter_part_units);
      }
      delay(unit * letter_space_units);
    }
  }
}
