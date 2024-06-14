#ifndef GUIT_H
#define GUIT_H

#include <string>
#include <vector>

using namespace std;

void help();
void initRepo(const string& name);
void addFiles(const string& options, const string& name);
string generarCommitId();
void commitChanges(const string& message);
void showStatus(const string& file);
void rollbackFile(const string& file, const string& commit);
void resetFile(const string& file);
void syncFile(const string& file);

string read_file(const string& file_name);

void write_file(const string& file_name, const string& content);

#endif // GUIT_H
