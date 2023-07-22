#include <iostream>
#include <windows.h>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;
string folder_name_to_find;
bool folder_found = false;
mutex folder_found_mutex;
void search_in_directory(const string& directory) {
    if (folder_found) {
        return;
    }
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (folder_found) {
            break;
        }

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            string sub_directory = findFileData.cFileName;
            if (sub_directory != "." && sub_directory != "..") {
                if (sub_directory == folder_name_to_find) {
                    lock_guard<mutex> lock(folder_found_mutex);
                    if (!folder_found) {
                        folder_found = true;
                        cout << "Folder found: " << directory + "\\" + findFileData.cFileName << endl;
                    }
                } else {
                    if (folder_found) {
                        break;
                    }
                    thread(search_in_directory, directory + "\\" + sub_directory).detach();
                }
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int main() {
    string root_directory = "C:\\";
    cout << "Enter the folder name you want to find: ";
    cin >> folder_name_to_find;
    search_in_directory(root_directory);
    this_thread::sleep_for(chrono::seconds(2));
    return 0;
}






