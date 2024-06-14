#ifndef GUIT_H
#define GUIT_H

#include <string>
#include <vector>
#include <filesystem>

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

#endif // GUIT_H
