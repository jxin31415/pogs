#pragma once

#include "util.h"
#include "enumerate.h"
#include "game.h"

using namespace std;

vector<vector<int>> rand_games(20);
set<int> moves_allowed;
void setup_rand_games(vector<int>& valid_moves) {
    for(int each: valid_moves) {
        moves_allowed.insert(each);
    }

    for(int t = 0; t < rand_games.size(); t++) {
        vector<int> game(100);
        for(int i = 0; i < 100; i++) {
            game[i] = valid_moves[(int) (rand() % valid_moves.size())];
        }
        rand_games[t] = game;
    }
}

ast_ptr cur_node;
bool valid_strategy;
int hole_filler(vector<string> vars, vector<int> vals) {
    envt env;
    for(int i = 0; i < vars.size(); i++) {
        env.set(vars[i], vals[i]);
    }

    int move = cur_node->interpret(env);

    // Validator: moves must be valid
    if(moves_allowed.count(move) == 0) { // Illegal move! Program is impossible
        valid_strategy = false;
        return *(moves_allowed.begin());
    }
    return move;
}

bool verifier(ast_ptr node, string game_str, vector<ast_ptr> nums, vector<ast_ptr> bools) {

    // Phase 1: play against a random opponent
    cur_node = node;
    valid_strategy = true;
    for(int i = 0; i < rand_games.size(); i++) {
        bool res = game(rand_games[i], &hole_filler);
        if(RANDOM_OPPONENT && !res) { // Game lost! Strategy doesn't work
            return false;
        }
        if(!valid_strategy) { // Generates invalid moves!
            return false;
        }
    }

    cout << node->to_string() << " strategy passes random opponent!" << endl;
    // Phase 2: play against adversarial opponent
    // -- Future work -- 

    // Phase 3: formally verify
    size_t inv_pos = game_str.find("?inv");
    for(int inv = 0; inv < bools.size(); inv++) {
        string copy = game_str;
        copy.replace(inv_pos, 4, bools[inv]->to_string());

        string out_file = "fun_verifier/in/in" + to_string(inv) + ".fun";
        ofstream out(out_file);
        out << copy;
        out.close();
    }

    system("cd fun_verifier/; ant -S >nul 2>nul");

    string res;
    ifstream res_file("fun_verifier/result.out");

    if(getline(res_file, res)) {
        int inv = stoi(res);
        cout << node->to_string() << " strategy passes formal verification with invariant " << bools[inv]->to_string() << "!" << endl;
        res_file.close();
        return true;
    }
    
    res_file.close();
    // Verifiably correct strategy not found :(
    return false;
}