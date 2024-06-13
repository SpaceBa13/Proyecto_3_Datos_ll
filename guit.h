#ifndef GUIT_H
#define GUIT_H

#include <string>

void help();
void initRepo(const std::string& name);
void addFiles(const std::string& options, const std::string& name);
void commitChanges(const std::string& message);
void showStatus(const std::string& file);
void rollbackFile(const std::string& file, const std::string& commit);
void resetFile(const std::string& file);
void syncFile(const std::string& file);

#endif // GUIT_H