#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

const long long CELLS_LONG = 30000, BUG_STOP = 10000000;

bool bracketOk(const std::string& code, std::map<int, int>& jump) {
    int opens = 0;
    for(int i = 0; i < code.size(); i++) {
        if(code[i] == '[') {
            opens+=1;
            jump[i] = i;
        }
        else if (code[i] == ']') {
            opens-=1;
            if (opens < 0) {
                return false;
            }
            for (int j = i-1; 0<=j; j--) {
                if (jump[j]==j && code[j] == '[') {
                    jump[j] = i;
                    jump[i] = j;
                    break;
                }      
            }
        }
    }
    return opens == 0;
}
int Method_1_out(std::string& code,int& count) {
    std::ofstream out("out.cpp");
    out << "#include <iostream>";
    out << "\nint main() {";
    out << "\nunsigned char tape[30000] = {0};";     
    out << "\nint p = 0;";
    for (unsigned int i = 0; i < code.size(); i++) {
        count++;
        if (count > BUG_STOP) {
            std::cout << "Too many!\n";
            return 2;
        }
        switch (code[i]) {
            case '+':
                out << "\ntape[p]++;";    
                break;  
            case '-':
                out << "\ntape[p]--;";
                break;
            case ',':
                out << "\ntape[p] = std::cin.get();";
                break;
            case '.':
                out << "\nstd::cout << tape[p];";
                break;
            case '>':
                out << "\np++;";
                break;
            case '<':
                out << "\np--;";
                break;
            case '[':
                out << "\nwhile (tape[p]) {";
                break;
            case ']':
                out << "\n}"; 
                break;  
            default:
                break;
            }
        }
    out << "\nreturn 0;\n}";
    std::cout << "Check out.cpp!";
    return 0;
}        
int Method_2_cons(std::string& code,int& count, std::map<int,int>& jump) {
    std::vector<unsigned char> cells(CELLS_LONG, 0);
    int p = 0, c = 0;
    for (unsigned int i = 0; i < code.size(); i++) {
        count++;
        if (count > BUG_STOP) {
            std::cout << "Too many!\n";
            return 2;
        }
        switch (code[i]) {
            case '+':
                cells[p]++;      
                break;  
            case '-':
                cells[p]--;
                break;
            case ',':
                c = std::cin.get();
                if (c == -1) {
                    cells[p] = 0;
                }
                else {
                    cells[p] = c;
                }
                break;
            case '.':
                std::cout << (char)cells[p];
                break;
            case '>':
                p++;
                if (p > cells.size()-1) {
                    p = 0;
                }
                break;
            case '<':
                p--;
                if (p < 0) {
                    p = cells.size()-1;
                }
                break;
            case '[':
                if (cells[p] == 0) {
                    i = jump[i];  
                }
                break;  
            case ']':
                if (cells[p] != 0) {
                    i = jump[i];  
                }      
                break;  
            default:
                break;
        }
    }
    return 0;
}
int main() {
    int method = 0, count =0;
    std::map<int, int> jump;
    std::cout << "What do you want to do?\n" << "1 - convert to C++\n" << "2 - run now\n";
    std::cin >> method;
    std::string code;
    if ( method != 1 && method != 2) {
        std::cout << "Invalid mode\n";
        return 1;
    }
    std::cin.get();
    std::cout << "Enter your code: " << '\n';
    std::getline(std::cin, code);
    if (!bracketOk(code, jump)) {
        std::cout << "Wrong Cycle\n";
        return 1;
    }
    if (method == 1) {
        return Method_1_out(code, count);
    }
    return Method_2_cons(code, count, jump);
}

