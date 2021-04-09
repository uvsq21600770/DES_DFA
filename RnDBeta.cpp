#include <bits/stdc++.h>
#include <bitset>
using namespace std;

#define cipherAmount 32

string hex2bin(string s)
{
    // hexadecimal to binary conversion
    unordered_map<char, string> mp;
    mp['0'] = "0000";
    mp['1'] = "0001";
    mp['2'] = "0010";
    mp['3'] = "0011";
    mp['4'] = "0100";
    mp['5'] = "0101";
    mp['6'] = "0110";
    mp['7'] = "0111";
    mp['8'] = "1000";
    mp['9'] = "1001";
    mp['A'] = "1010";
    mp['B'] = "1011";
    mp['C'] = "1100";
    mp['D'] = "1101";
    mp['E'] = "1110";
    mp['F'] = "1111";
    string bin = "";
    for (long unsigned int i = 0; i < s.size(); i++) {
        bin += mp[s[i]];
    }
    return bin;
}
string bin2hex(string s)
{
    // binary to hexadecimal conversion
    unordered_map<string, string> mp;
    mp["0000"] = "0";
    mp["0001"] = "1";
    mp["0010"] = "2";
    mp["0011"] = "3";
    mp["0100"] = "4";
    mp["0101"] = "5";
    mp["0110"] = "6";
    mp["0111"] = "7";
    mp["1000"] = "8";
    mp["1001"] = "9";
    mp["1010"] = "A";
    mp["1011"] = "B";
    mp["1100"] = "C";
    mp["1101"] = "D";
    mp["1110"] = "E";
    mp["1111"] = "F";
    mp["00"] = "0";
    mp["01"] = "1";
    mp["10"] = "2";
    mp["11"] = "3";
    string hex = "";
    long unsigned int i = 0;

    if(s.length() == 6)
    {
      string ch = "";
      ch += s[i];
      ch += s[i + 1];
      i += 2;

      hex += mp[ch];
    }
    for (; i < s.length(); i += 4) {
        string ch = "";
        ch += s[i];
        ch += s[i + 1];
        ch += s[i + 2];
        ch += s[i + 3];
        hex += mp[ch];
    }
    return hex;
}

string permute(string k, int* arr, int n)
{
    string per = "";
    for (int i = 0; i < n; i++) {
        per += k[arr[i] - 1];
    }
    return per;
}

string shift_left(string k, int shifts)
{
    string s = "";
    for (int i = 0; i < shifts; i++) {
        for (int j = 1; j < 28; j++) {
            s += k[j];
        }
        s += k[0];
        k = s;
        s = "";
    }
    return k;
}

string xor_(string a, string b)
{
    string ans = "";
    for (long unsigned int i = 0; i < a.size(); i++) {
        if (a[i] == b[i]) {
            ans += "0";
        }
        else {
            ans += "1";
        }
    }
    return ans;
}
string encrypt(string pt, vector<string> rkb, vector<string> rk)
{
    // Hexadecimal to binary
    pt = hex2bin(pt);

    // Initial Permutation Table
    int initial_perm[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                             60, 52, 44, 36, 28, 20, 12, 4,
                             62, 54, 46, 38, 30, 22, 14, 6,
                             64, 56, 48, 40, 32, 24, 16, 8,
                             57, 49, 41, 33, 25, 17, 9, 1,
                             59, 51, 43, 35, 27, 19, 11, 3,
                             61, 53, 45, 37, 29, 21, 13, 5,
                             63, 55, 47, 39, 31, 23, 15, 7 };
    // Initial Permutation
    pt = permute(pt, initial_perm, 64);
    //cout << "After initial permutation: " << bin2hex(pt) << endl;

    // Splitting
    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);
    //cout << "After splitting: L0=" << bin2hex(left)
      //   << " R0=" << bin2hex(right) << endl;

    // Expansion D-box Table
    int exp_d[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                      6, 7, 8, 9, 8, 9, 10, 11,
                      12, 13, 12, 13, 14, 15, 16, 17,
                      16, 17, 18, 19, 20, 21, 20, 21,
                      22, 23, 24, 25, 24, 25, 26, 27,
                      28, 29, 28, 29, 30, 31, 32, 1 };

    // S-box Table
    int s[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },

                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

    // Straight Permutation Table
    int per[32] = { 16, 7, 20, 21,
                    29, 12, 28, 17,
                    1, 15, 23, 26,
                    5, 18, 31, 10,
                    2, 8, 24, 14,
                    32, 27, 3, 9,
                    19, 13, 30, 6,
                    22, 11, 4, 25 };

    cout << endl;
    for (int i = 0; i < 16; i++) {
        // Expansion D-box
        string right_expanded = permute(right, exp_d, 48);

        // XOR RoundKey[i] and right_expanded
        string x = xor_(rkb[i], right_expanded);

        // S-boxes
        string op = "";
        for (int i = 0; i < 8; i++) {
            int row = 2 * int(x[i * 6] - '0') + int(x[i * 6 + 5] - '0');
            int col = 8 * int(x[i * 6 + 1] - '0') + 4 * int(x[i * 6 + 2] - '0') + 2 * int(x[i * 6 + 3] - '0') + int(x[i * 6 + 4] - '0');
            int val = s[i][row][col];
            op += char(val / 8 + '0');
            val = val % 8;
            op += char(val / 4 + '0');
            val = val % 4;
            op += char(val / 2 + '0');
            val = val % 2;
            op += char(val + '0');
        }
        // Straight D-box
        op = permute(op, per, 32);

        // XOR left and op
        x = xor_(op, left);

        left = x;

        // Swapper
        if (i != 15) {
            swap(left, right);
        }
        cout << "Round " << i + 1 << " " << bin2hex(left) << " "
            << bin2hex(right) << " " << rk[i] << endl;
    }

    // Combination
    string combine = left + right;

    // Final Permutation Table
    int final_perm[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                           39, 7, 47, 15, 55, 23, 63, 31,
                           38, 6, 46, 14, 54, 22, 62, 30,
                           37, 5, 45, 13, 53, 21, 61, 29,
                           36, 4, 44, 12, 52, 20, 60, 28,
                           35, 3, 43, 11, 51, 19, 59, 27,
                           34, 2, 42, 10, 50, 18, 58, 26,
                           33, 1, 41, 9, 49, 17, 57, 25 };

    // Final Permutation
    string cipher = bin2hex(permute(combine, final_perm, 64));
    return cipher;
}

string IP_INV(string CipherHex)
{

 int initial_perm[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
                          60, 52, 44, 36, 28, 20, 12, 4,
                          62, 54, 46, 38, 30, 22, 14, 6,
                          64, 56, 48, 40, 32, 24, 16, 8,
                          57, 49, 41, 33, 25, 17, 9, 1,
                          59, 51, 43, 35, 27, 19, 11, 3,
                          61, 53, 45, 37, 29, 21, 13, 5,
                          63, 55, 47, 39, 31, 23, 15, 7 };
  // Final Permutation
  string cipher = permute(hex2bin(CipherHex), initial_perm, 64);
  return bin2hex(cipher);
}

std::string int2hex(unsigned long int i , int flushSize = 16)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(flushSize)
         << std::hex << i;

  string key = stream.str();
  for (string::iterator i = key.begin(); i != key.end(); i++) {
   *i = toupper(*i);
  }
  return key;
}

string F_Perm_Inv(string cipher)
{
  int inv_per[32] = { 9, 17, 23, 31,
                      13, 28, 2, 18,
                      24, 16, 30, 6,
                      26, 20, 10, 1,
                      8, 14, 25, 3,
                      4, 29, 11, 19,
                      32, 12, 22, 7,
                      5, 27, 15, 21 };

  string res = permute(cipher, inv_per, 32);

  return res;
}

string expend(string word)
{
  int exp_d[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                    6, 7, 8, 9, 8, 9, 10, 11,
                    12, 13, 12, 13, 14, 15, 16, 17,
                    16, 17, 18, 19, 20, 21, 20, 21,
                    22, 23, 24, 25, 24, 25, 26, 27,
                    28, 29, 28, 29, 30, 31, 32, 1 };

  string expended_word = permute(word, exp_d, 48);
  return expended_word;
}


void SBox_Cutting(string SBox_In, int offset, string SBox_Cut[])
{
  for(int i = 0; i < 8; i++)
  {
    SBox_Cut[i] = SBox_In.substr(i*offset,offset);
  }

}

void display_SBox_In_Out(string SBox[])
{
  for(int i = 0; i < 8; i++)
  {
    cout << "["<< i << "] " << SBox[i] << endl;
  }
}

int bin2int(string bin)
{
  int res = 0;
  int base = 1;
  int len = bin.size() - 1;
  for (int i = len; i >= 0; i--) {
      res += (bin[i] - 48) * base;
      base = base * 2;
  }
  return res;
}

string int2bin(int a, int size = 6)
{
  string bin;
  switch(size)
  {
    case 6:
       bin = bitset<6>(a).to_string();
      break;
    case 4:
       bin = bitset<4>(a).to_string();
      break;
    case 8:
       bin = bitset<8>(a).to_string();
       break;
    case 2:
       bin = bitset<2>(a).to_string();
       break;
  }

  return bin;
}

int getRow(string input)
{
  string row = "";
  row += input[0];
  row += input[5];
  return bin2int(row);
}

int getColumn(string input)
{
  string column = "";
  column = input.substr(1,4);
  return bin2int(column);
}

string decompressKey(string K16)
{
  int key_comp[48] = { 14, 17, 11, 24, 1, 5,
                       3, 28, 15, 6, 21, 10,
                       23, 19, 12, 4, 26, 8,
                       16, 7, 27, 20, 13, 2,
                       41, 52, 31, 37, 47, 55,
                       30, 40, 51, 45, 33, 48,
                       44, 49, 39, 56, 34, 53,
                       46, 42, 50, 36, 29, 32 };

   string K16_extended(56, 'x');
   cout << "K16:" << endl << K16 << endl;
   cout << "Inverted init:" << endl << K16_extended << endl;
   for(int i = 0; i < 48; i++)
   {
     K16_extended[key_comp[i] - 1] = K16[i];
   }
   cout << "Inverted filled:" << endl << K16_extended << endl;

   return K16_extended;
}

string INV_PC1(string K16_extended)
{
  int keyp[56] = { 57, 49, 41, 33, 25, 17, 9,
                   1, 58, 50, 42, 34, 26, 18,
                   10, 2, 59, 51, 43, 35, 27,
                   19, 11, 3, 60, 52, 44, 36,
                   63, 55, 47, 39, 31, 23, 15,
                   7, 62, 54, 46, 38, 30, 22,
                   14, 6, 61, 53, 45, 37, 29,
                   21, 13, 5, 28, 20, 12, 4 };
  string K16_PC1(64, '!');
  for(int i = 0; i < 56; i++)
  {
    K16_PC1[keyp[i] - 1] = K16_extended[i];
  }
  cout << "PC1-1" << endl;
  cout << K16_extended << endl;
  cout << K16_PC1 << endl << endl;
  return K16_PC1;
}

string keyGen(string partial_K16, int currentGuess, int unknownKeyBits[8])
{
  string guess = int2bin(currentGuess,8);
  cout << "guess is" << guess << endl;
  cout << "Orig Partial K16" << endl << partial_K16 << endl;
  for(int i = 0; i < 8; i++)
  {
      partial_K16[unknownKeyBits[i]] = guess[i];
  }
  cout << "partial K16 is" << endl << partial_K16 << endl;
  int parity = 0;

  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 7; j++)
    {
      parity += partial_K16[8*i + j] - 48;
    }
    partial_K16[(i+1) * 8 - 1] = 48 + (parity+1)%2;
    parity = 0;
  }
  cout << partial_K16.size() << endl;
  return partial_K16;
}

string DES(string pt, string key)
{

  // Hex to binary
  key = hex2bin(key);
  // Parity bit drop table
  int keyp[56] = { 57, 49, 41, 33, 25, 17, 9,
                   1, 58, 50, 42, 34, 26, 18,
                   10, 2, 59, 51, 43, 35, 27,
                   19, 11, 3, 60, 52, 44, 36,
                   63, 55, 47, 39, 31, 23, 15,
                   7, 62, 54, 46, 38, 30, 22,
                   14, 6, 61, 53, 45, 37, 29,
                   21, 13, 5, 28, 20, 12, 4 };

  // getting 56 bit key from 64 bit using the parity bits
  key = permute(key, keyp, 56); // key without parity

  // Number of bit shifts
  int shift_table[16] = { 1, 1, 2, 2,
                          2, 2, 2, 2,
                          1, 2, 2, 2,
                          2, 2, 2, 1 };

  // Key- Compression Table
  int key_comp[48] = { 14, 17, 11, 24, 1, 5,
                       3, 28, 15, 6, 21, 10,
                       23, 19, 12, 4, 26, 8,
                       16, 7, 27, 20, 13, 2,
                       41, 52, 31, 37, 47, 55,
                       30, 40, 51, 45, 33, 48,
                       44, 49, 39, 56, 34, 53,
                       46, 42, 50, 36, 29, 32 };

  // Splitting
  string left = key.substr(0, 28);
  string right = key.substr(28, 28);

  vector<string> rkb; // rkb for RoundKeys in binary
  vector<string> rk; // rk for RoundKeys in hexadecimal
  for (int i = 0; i < 16; i++) {
      // Shifting
      left = shift_left(left, shift_table[i]);
      right = shift_left(right, shift_table[i]);

      // Combining
      string combine = left + right;

      // Key Compression
      string RoundKey = permute(combine, key_comp, 48);

      rkb.push_back(RoundKey);
      rk.push_back(bin2hex(RoundKey));
  }

  //cout << "\nEncryption:\n\n";
  string cipher = encrypt(pt, rkb, rk);

  return cipher;
}

/*
  Check that SI is modified back in the calling function
*/
void reverseSBox(int SO, int numSBox, string SI[])
{
  int s[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                      { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },

                      { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                      { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                      { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                      { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                      { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                      { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

   for(int i = 0; i < 4; i++)
   {
     SI[i] = "";
     for(int j = 0; j < 16; j++)
     {
       if(SO == s[numSBox][i][j])
       {
         string i_string = int2bin(i, 2);
         string j_string = int2bin(j, 4);
         SI[i] += i_string[0];
         SI[i] += j_string;
         SI[i] += i_string[1];
       }
     }
   }
}

int execSBox(int input, int numSBox)
{
  int s[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                      { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },

                      { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                      { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                      { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                      { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                      { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                      { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

  string input_string = int2bin(input);
  int row = getRow(input_string);
  int col = getColumn(input_string);

  return s[numSBox][row][col];
}

int intersection(int selectionKey[64])
{
  int rang_max = 0;
  int max = selectionKey[0];
  for(int i = 0; i < 64; i++)
  {
    if(max < selectionKey[i])
    {
      max = selectionKey[i];
      rang_max = i;
    }
  }

  return rang_max;
}

/*
ER15_full: ER15 du cipher correct, après expension et avant Découpage
F_ER15_full[7]: Tableau d'ER15 des ciphers fautés après expension et avant Découpage
SBox_Out_full[32]: Tableau de P-1(R16 XOR F_R16) avant découpage

*/
string sbox_Attack(string ER15_full, string F_ER15_full[cipherAmount], string SBox_Out_full[cipherAmount], int numSBox)
{
  int candidateKey[32][65];
  int selectionKey[64];
  for(int i = 0; i < 32; i++)
  {
    for(int j = 0; j < 64; j++)
    {
      candidateKey[i][j] = 0;
    }

    selectionKey[i] = 0;
    selectionKey[i+32] = 0;
    candidateKey[i][64] = 1;
  }

  bool skip = false;
  string ER15[8];
  string F_ER15[cipherAmount][8];
  string SBox_Out[cipherAmount][8];
  SBox_Cutting(ER15_full, 6, ER15);

  int K16_fragment[4];

  for(int currentCipher = 0; currentCipher < cipherAmount; currentCipher++)
  {
    skip = false;
    SBox_Cutting(F_ER15_full[currentCipher], 6, F_ER15[currentCipher]);
    SBox_Cutting(SBox_Out_full[currentCipher], 4, SBox_Out[currentCipher]);

    if(bin2int(SBox_Out[currentCipher][numSBox]) == 0)
    {
      skip = true;
      candidateKey[currentCipher][64] = 0;
    }

    if(ER15_full.compare(F_ER15[currentCipher][numSBox]) == 0)
    {
      skip = true;
      candidateKey[currentCipher][64] = 0;
    }

    if(!skip)
    {
      string SI[4];
      for(int SO = 0; SO < 16; SO++)
      {
        reverseSBox(SO, numSBox, SI);
        K16_fragment[0] = bin2int(SI[0]) ^ bin2int(ER15[numSBox]);
        K16_fragment[1] = bin2int(SI[1]) ^ bin2int(ER15[numSBox]);
        K16_fragment[2] = bin2int(SI[2]) ^ bin2int(ER15[numSBox]);
        K16_fragment[3] = bin2int(SI[3]) ^ bin2int(ER15[numSBox]);

        int So_F_ER15[4];
        for(int k = 0; k < 4; k++)
        {
          int F_ER15_xor_K16 = bin2int(F_ER15[currentCipher][numSBox]) ^ K16_fragment[k];

          So_F_ER15[k] = execSBox(F_ER15_xor_K16, numSBox);

          if(bin2int(SBox_Out[currentCipher][numSBox]) == (So_F_ER15[k] ^ SO))
          {
            candidateKey[currentCipher][K16_fragment[k]]++;
            selectionKey[K16_fragment[k]]++;
          }

        }
      }
    }

  }

  cout << "Possible Sub-Keys fragments for SBox " << numSBox << endl;

  for(int currentCipher = 0; currentCipher < cipherAmount; currentCipher++)
  {
    int ct = 0;
    if(candidateKey[currentCipher][64] == 1)
    {
      cout << "On cipher n°" << currentCipher << endl;
      for(int j = 0; j < 64; j++)
      {
        if(candidateKey[currentCipher][j] != 0)
        {
          ct++;
          cout << int2hex(j, 2) << "   " << int2bin(j) << endl;
        }
      }
      cout << "Amount of possible fragments:" << ct << endl << endl;
    }
  }

  int part = intersection(selectionKey);
  cout << "PART " << part << "   " << int2hex(part, 2) << " -------------" << endl << endl << endl;

  return int2bin(part);
}


int main()
{
  string plainText = "949EAD012F342901";
  string cipher = "45F1CC01273F3139";

  string C16 = IP_INV(cipher);
  string L16 = C16.substr(0, 8);
  string R16 = C16.substr(8, 8);
  cout << L16 << " " << R16 << endl;

  string R15 = R16;
  R16 = L16;
  R16 = hex2bin(R16);
  R15 = hex2bin(R15);

  //S_In
  R15 = expend(R15);

  //The string will contain a binary representation of K16
  string K16 = "";

  string faulty_cipher[cipherAmount] =
    {
      "47E4CC05273F3139",
      "45E3CC05273E3139",
      "45F1CE45273F3139",
      "44A1C847373F3139",
      "45A1CC45353E3139",
      "44B1CC01273D3139",
      "45B1C801373F3339",
      "44B1C800673F313B",
      "4CB1C801773B3139",
      "45F9CC00673F3139",
      "45F1C401672F3139",
      "05F1DC08273B3138",
      "05F1CC016F2F3138",
      "05F1CC0127773138",
      "45F1DC01273F3938",
      "45F1DC01277F3070",
      "25F1CC01237F3079",
      "45D1CC01233F2179",
      "45F1EC01273F2039",
      "51F1CC21233F2139",
      "41F18D01073F2079",
      "41F1CD01271F3139",
      "45F18D01273F1139",
      "55F18C11273F7119",
      "D1F18C01263F7139",
      "4571CC11263F3139",
      "45F14C11273F3539",
      "45F4CC91273F7529",
      "45F4CC11A63F312D",
      "45F4CC0127BF3129",
      "45F0CC01273FB12D",
      "45F0CC41273E31A9"
    };

    string F_ER15[cipherAmount];
    string SBox_Out_full[cipherAmount];
    //string F_R16[7];

    for(int i = 0; i < cipherAmount; i++)
    {
      string F_C16 = IP_INV(faulty_cipher[i]);
      string F_L16 = F_C16.substr(0, 8);
      string F_R16 = F_C16.substr(8, 8);
      string F_R15 = F_R16;
      F_R16 = F_L16;
      F_R16 = hex2bin(F_R16);
      F_R15 = hex2bin(F_R15);

      F_ER15[i] = expend(F_R15);
      SBox_Out_full[i] = xor_(R16, F_R16);
      //S_out
      SBox_Out_full[i] = F_Perm_Inv(SBox_Out_full[i]);
    }

    for(int numSBox = 0; numSBox < 8; numSBox++)
    {
      K16 += sbox_Attack(R15, F_ER15, SBox_Out_full, numSBox);
    }

    cout << bin2hex(K16) << endl;
    cout << "K16 regular: " << K16.size() << endl << K16 << endl;
    K16 = decompressKey(K16);
    cout << "K16 Decomp: " << K16.size() << endl << K16 << endl;
    K16 = INV_PC1(K16);
    cout << "K16 Inv: " << endl << K16 << endl;

    int unknownKeyBits[8] = {13, 14, 18, 19, 50, 53, 57, 59};
    int currentGuess = 0;
    bool keyFound = false;

    while(currentGuess < 256 && !keyFound)
    {
      string Key = keyGen(K16, currentGuess, unknownKeyBits);
      currentGuess++;
      Key = bin2hex(Key);
      string cipherGuess = DES(plainText, Key);
      if(cipher.compare(cipherGuess) == 0)
        {
          cout << "found the key" << endl;
          cout << Key << endl << hex2bin(Key) << endl;
          keyFound = true;
        }
    }
    return 0;
}
