#include <bits/stdc++.h>

using namespace std;

const char* to_const_char_star(string s) {
    char* buffer = new char[s.length() + 1];
    for(int i = 0; i < s.length(); i++) {
        buffer[i] = s[i];
    }
    buffer[s.length()] = '\0';
    return buffer;
}

void shell(string instruction) {
    system(to_const_char_star(instruction));
}

int file_no = 0;

void test_instruction(string prog, string filename, string path, int counter, float sleep_time = 10) {
    string ins1 = "cd " + path + " && sudo perf stat -e cycles,instructions,cache-references,cache-misses,bus-cycles -o " + filename + string("_out1.txt ") + prog + " sleep " + to_string(sleep_time);
    string ins2 = "cd " + path + " && sudo perf stat -e L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores -o " + filename + string("_out2.txt ")  + prog + " sleep " + to_string(sleep_time);
    string ins3 = "cd " + path + " && sudo perf stat -e dTLB-loads,dTLB-load-misses -o " + filename+ string("_out3.txt ")  + prog + " sleep " + to_string(sleep_time);
    shell(ins1);
    shell(ins2);
    shell(ins3);
    
    ifstream fin; ofstream fout;
    shell("mv " + path + "/" + filename + "_out1.txt " + filename + "_out1.txt");
    fin.open(filename + "_out1.txt");
    fout.open(filename + "_res" + to_string(counter) + ".txt");
    string line;
    
    while(getline(fin,line))
    {
        fout<<line<<endl;
    }
    
    fout<<"out1 ends here"<<endl;
    fin.close();
    shell("mv " + path + "/" + filename + "_out2.txt " + filename + "_out2.txt");
    fin.open(filename + "_out2.txt");
    
    while(getline(fin,line))
    {
        fout<<line<<endl;
    }
    
    fout<<"out2 ends here"<<endl;
    fin.close();
    shell("mv " + path + "/" + filename + "_out3.txt " + filename + "_out3.txt");
    fin.open(filename + "_out3.txt");
    
    while(getline(fin,line))
    {
        fout<<line<<endl;
    }
    
    fout<<"out3 ends here"<<endl;
    fin.close();
    fout.close();
    shell("mkdir " + filename);
    shell("mv " + filename + "_res" + to_string(counter) + ".txt " + filename + "/" + filename + "_res" + to_string(counter) + ".txt");
    shell("rm -rf " + filename + "_out1.txt");
    shell("rm -rf " + filename + "_out2.txt");
    shell("rm -rf " + filename + "_out3.txt");
}

void test_meltdown() {
    shell("cd meltdown && make");
    vector<string> all_progs = {"taskset 0x1 ./test", "sudo taskset 0x1 ./kaslr", "sudo taskset 0x1 ./reliability 0xffff880000000000", 
                                    "sudo ./secret"};

    vector<string> all_names = {"test", "kalsr", "reliability", "secret"};
    string path = "~/cs3500/project/meltdown";
    for(int i = 0; i < 40; i++) {
        for(int i = 0; i < all_progs.size(); i++) {
            test_instruction(all_progs[i], all_names[i], path, i, 0.0001);
        }
        shell("cd meltdown && ./memory_filler 9");
        test_instruction("taskset 0x1 ./memdump 0x240000000 -1 0xffff880000000000", "memdump", path, i, 0.0001);
    }
    shell("mkdir MELTDOWN");
    for(auto name : all_names) {
        shell("cp -r " + name + " MELTDOWN/");
        shell("rm -rf " + name);
    }

    shell("mv memdump MELTDOWN/");
}

void run_help(string path, string makepath, string name, int counter)
{
    shell("cd " + makepath + "&& make");
    string small = "bash runme_small.sh";
    string large = "bash runme_large.sh";
    test_instruction(small,name+"small", path, counter);
    test_instruction(large,name+"large", path, counter);
}


void test_mibench() {
    // mibench
    // automative-> basicmath, bitcount, qsort
    // office -> stringsearch
    // network -> dijkstra, patrica
    // security -> blowfish, sha
    // telecom -> FFT, CRC32
    vector<string> normal_prog_list = {"basicmath", "bitcount", "quicksort", "stringsearch", "dijkstra", "patricia", "blowfish", "sha", "FFT", "CRC32"};
    for(int i = 0; i < 20; i++) {
        run_help("~/cs3500/project/mibench/automotive/basicmath", "~/cs3500/project/mibench/automotive/basicmath",  "basicmath", i);
        run_help("~/cs3500/project/mibench/automotive/bitcount", "~/cs3500/project/mibench/automotive/bitcount",  "bitcount", i);
        run_help("~/cs3500/project/mibench/automotive/qsort", "~/cs3500/project/mibench/automotive/qsort",  "quicksort", i);
        run_help("~/cs3500/project/mibench/office/stringsearch", "~/cs3500/project/mibench/office/stringsearch",  "stringsearch", i);
        run_help("~/cs3500/project/mibench/network/dijkstra", "~/cs3500/project/mibench/network/dijkstra",  "dijkstra", i);
        run_help("~/cs3500/project/mibench/network/patricia", "~/cs3500/project/mibench/network/patricia",  "patricia", i);
        run_help("~/cs3500/project/mibench/security/blowfish", "~/cs3500/project/mibench/security/blowfish",  "blowfish", i);
        run_help("~/cs3500/project/mibench/security/sha", "~/cs3500/project/mibench/security/sha",  "sha", i);
        run_help("~/cs3500/project/mibench/telecomm/FFT", "~/cs3500/project/mibench/telecomm/FFT",  "FFT", i);
        run_help("~/cs3500/project/mibench/telecomm/CRC32", "~/cs3500/project/mibench/telecomm/CRC32",  "CRC32", i);
    }
    shell("mkdir MIBENCH");
    for(auto name : normal_prog_list) {
        shell("cp -r " + name + "small MIBENCH/");
        shell("rm -rf " + name + "small");
        shell("cp -r " + name + "large MIBENCH/");
        shell("rm -rf " + name + "large");
    }
}

void test_rowhammer() {
    shell("cd rowhammer-test && ./make.sh");
    string path = "~/cs3500/project/rowhammer-test";
    for(int i = 0; i < 10; i++) {
        test_instruction("./rowhammer_test", "rowhammer", path, i, 0.0001);
    }  
    shell("mkdir ROWHAMMER_TEST");
    shell("cp -r rowhammer ROWHAMMER_TEST/");
    shell("rm -rf rowhammer");
}

int main() {
    shell("sudo modprobe msr");
    // test_mibench();
    // test_meltdown();
    // test_rowhammer();    
}
