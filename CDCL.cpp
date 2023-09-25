//Written by F2183, 
//please do not copy directly and make sure you write your own code after understanding the meaning of the code, 
//the author will not be responsible if it causes a massive code plagiarism check by the faculty.
//Due to the problems mentioned above, the author will not provide code comments.
#include <iostream>
#include <cmath>
using namespace std;

enum Assignment { F = -1, unassigned, T };
enum Source { pick, unitpro, unknown };
int num_clause;
int num_variable;

struct Clause
{
    int n;
    int literal[1000]{}; 
};

struct Variable
{
    int i;
    Assignment a;
    Source s;
    Variable* next = nullptr;
};

struct Backtrack
{
    Source s = unknown;
    int source_unitpro[1000]{};
};

Clause clause[100000]; 
Variable* mov, * rear, * p; 
Assignment assignment[1000] = { unassigned }; 
Backtrack backtrack[1000];

bool Conflict(Assignment assignment[], Clause clause[], Clause* lct)
{
    for (int i = 0; i < num_clause; i++)
    {
        bool flag = false;
        for (int j = 0; j < clause[i].n; j++)
        {
            if (clause[i].literal[j] * assignment[abs(clause[i].literal[j])] >= 0)
            {
                flag = true;
            }
        }
        if (!flag)
        {
            *lct = clause[i];
            return true;
        }
    }
    return false;
}

void Back_track(int aj, Backtrack b, int& N, Clause& conflict)
{
    if (b.s == pick)
    {
        bool flag = false;
        for (int k = 0; k < N; k++)
        {
            if (abs(conflict.literal[k]) == aj)
            {
                flag = true;
            }
        }
        if (!flag)
        {
            conflict.literal[N] = -aj * assignment[aj];
            N += 1;
        }
        return;
    }
    if (b.s == unitpro)
    {
        for (int k = 0; k < 2 && b.source_unitpro[k] != 0; k++)
        {
            Back_track(b.source_unitpro[k], backtrack[b.source_unitpro[k]], N, conflict);
        }
    }
}

Clause Conflict_clause(Clause a)
{
    Clause conflict;
    int N = 0;
    for (int j = 0; j < a.n; j++)
    {
        Back_track(abs(a.literal[j]), backtrack[abs(a.literal[j])], N, conflict);
    }
    conflict.n = N;
    return conflict;
}

void Remove(Clause conflict)
{
    Variable* mov, * p;
    int sign;
    if (conflict.n == 1)
    {
        mov = rear;
        while (mov->i != abs(conflict.literal[0]))
        {
            mov = mov->next;
        }
        while (rear != mov)
        {
            p = rear->next;
            assignment[rear->i] = unassigned;
            backtrack[rear->i].s = unknown;
            for (int m = 0; m < 100; m++)
            {
                backtrack[rear->i].source_unitpro[m] = 0;
            }
            delete rear;
            rear = p;
        }
        p = mov = rear->next;
        assignment[rear->i] = unassigned;
        backtrack[rear->i].s = unknown;
        for (int m = 0; m < 100; m++)
        {
            backtrack[rear->i].source_unitpro[m] = 0;
        }
        delete rear;
        rear = p;
    }
    if (conflict.n >= 2)
    {
        mov = rear;
        sign = 0;
        while (sign != 2)
        {
            for (int j = 0; j < conflict.n; j++)
            {
                if (abs(conflict.literal[j]) == mov->i)
                {
                    sign++;
                }
            }
            mov = mov->next;
        }
        while (rear->next->next != mov)
        {
            p = rear->next;
            assignment[rear->i] = unassigned;
            backtrack[rear->i].s = unknown;
            for (int m = 0; m < 100; m++)
            {
                backtrack[rear->i].source_unitpro[m] = 0;
            }
            delete rear;
            rear = p;
        }
        p = rear->next;
        assignment[rear->i] = unassigned;
        backtrack[rear->i].s = unknown;
        for (int m = 0; m < 100; m++)
        {
            backtrack[rear->i].source_unitpro[m] = 0;
        }
        delete rear;
        rear = p;
        mov = p;
    }
}

void Unit_propagation(Assignment assignment[], Clause clause[])
{
    int sign;
    for (int i = 0; i < num_clause; i++)
    {
        sign = 0;
        for (int j = 0; j < clause[i].n; j++)
        {
            if (clause[i].literal[j] * assignment[abs(clause[i].literal[j])] < 0)
            {
                sign++;
            }
        }
        if (sign == clause[i].n - 1)
        {
            for (int j = 0; j < clause[i].n; j++)
            {
                if (assignment[abs(clause[i].literal[j])] == unassigned)
                {
                    if (clause[i].literal[j] > 0)
                    {
                        backtrack[abs(clause[i].literal[j])].s = unitpro;
                        int sign1 = 0;
                        for (int k = 0; k < clause[i].n; k++)
                        {
                            if (assignment[abs(clause[i].literal[k])] != unassigned)
                            {
                                backtrack[abs(clause[i].literal[j])].source_unitpro[sign1] = abs(clause[i].literal[k]);
                                sign1++;
                            }
                        }
                        assignment[abs(clause[i].literal[j])] = T;
                        p = new Variable;
                        p->i = abs(clause[i].literal[j]);
                        p->a = T;
                        p->s = unitpro;
                        p->next = rear;
                        rear = p;
                    }
                    if (clause[i].literal[j] < 0)
                    {
                        backtrack[abs(clause[i].literal[j])].s = unitpro;
                        int sign2 = 0;
                        for (int k = 0; k < clause[i].n; k++)
                        {
                            if (assignment[abs(clause[i].literal[k])] != unassigned)
                            {
                                backtrack[abs(clause[i].literal[j])].source_unitpro[sign2] = abs(clause[i].literal[k]);
                                sign2++;
                            }
                        }
                        assignment[abs(clause[i].literal[j])] = F;
                        p = new Variable;
                        p->i = abs(clause[i].literal[j]);
                        p->a = F;
                        p->s = unitpro;
                        p->next = rear;
                        rear = p;
                    }
                }
            }
        }
    }
}

bool Assignment_completed(Assignment assignment[])
{
    for (int i = 1; i <= num_variable; i++)
    {
        if (assignment[i] == unassigned)
        {
            return false;
        }
    }
    return true;
}

int Find_unassigned(Assignment assignment[])
{
    for (int i = 1; i <= num_variable; i++)
    {
        if (assignment[i] == unassigned)
        {
            return i;
        }
    }
    return 0;
}

int main()
{
    cout << "Please do not copy directly and make sure you write your own code after understanding the meaning of the code! " << endl;
    p = rear = mov = nullptr;
    cin >> num_variable >> num_clause;
    for (int i = 0, a, mid; i < num_clause; i++)
    {
        a = 0;
        cin >> mid;
        while (mid != 0)
        {
            clause[i].literal[a] = mid;
            a++;
            cin >> mid;
        }
        clause[i].n = a;
    }
    for (int i = 0; i < num_clause; i++)
    {
        if (!clause[i].n)
        {
            cout << "UNSAT";
            return 0;
        }
    }
    Clause d, D;
    while (1)
    {
        Unit_propagation(assignment, clause);
        bool flag = Conflict(assignment, clause, &d);
        if (flag)
        {
            D = Conflict_clause(d);
            if (!D.n)
            {
                cout << "UNSAT";
                return 0;
            }
            clause[num_clause] = D;
            num_clause++;
            Remove(D);
        }
        else
        {
            if (Assignment_completed(assignment))
            {
                cout << "ONE POSSIBLE ASSIGNMENT：" << endl;
                for (int i = 1; i <= num_variable; ++i)
                    cout << i << ": " << (assignment[i] == T ? 'T' : 'F') << '\t';
                return 0;
            }
            else
            {
                int P = Find_unassigned(assignment);
                assignment[P] = T;
                backtrack[P].s = pick;
                p = new Variable;
                p->i = P;
                p->a = T;
                p->s = pick;
                p->next = rear;
                rear = p;
            }
        }
    }
    return 0;
}


