//
//  main.cpp
//  OutputFolderStructure
//
//  Created by JuHeQi on 14-4-2.
//  Copyright (c) 2014年 JU Heqi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/filesystem.hpp>


std::vector<std::string> FolderStack;
std::map<std::string, size_t> FolderEntryLeft;

void DrawOutputTextLine(const std::string& fullPath, const size_t depth)
{
    boost::filesystem::path dir(fullPath);
    
    for (auto i = 0; i < depth; ++i)
    {
        std::cout << (i == 0 ? " ": "     ")
            << (FolderEntryLeft[FolderStack[i]] ? "|" : " ");
    }
    
    std::cout << "----"
        << ( boost::filesystem::is_directory(dir) ? "+" :
            ( ( FolderEntryLeft[dir.parent_path().string()] == 1 ) ? "\\" : "-" ) )
        << dir.filename() << std::endl;
    
    if(FolderEntryLeft[dir.parent_path().string()] > 0)
        --FolderEntryLeft[dir.parent_path().string()];
}

size_t GetFolderSubEntryCount(const std::string& folderFullPath)
{
    using namespace boost::filesystem;
    path dir(folderFullPath);
    
    return std::count_if(directory_iterator(dir), directory_iterator(),
                         [](const path&)->bool{return true;});
}

std::string CollectFiles(const std::string& path)
{
    std::string ret;
    
    boost::filesystem::path dir(path);
    boost::filesystem::directory_iterator it_end;
    
    for (boost::filesystem::directory_iterator it_dir(dir); it_dir != it_end; ++it_dir)
    {
        if (boost::filesystem::is_directory(it_dir->path()))
        {
            DrawOutputTextLine(it_dir->path().string(), FolderStack.size());
            
            if(FolderEntryLeft.find(it_dir->path().string()) == FolderEntryLeft.end())
            {
                FolderEntryLeft[it_dir->path().string()]
                = GetFolderSubEntryCount(it_dir->path().string());
            }
            
            FolderStack.push_back(it_dir->path().string());
            CollectFiles(it_dir->path().string());
            FolderStack.pop_back();
            
        }
        else
        {
            DrawOutputTextLine(it_dir->path().string(), FolderStack.size());
        }
    }
    
    return ret;
}

int main(int argc, const char * argv[])
{
    std::cout << "===OutputFolderStructure===\n";
    const std::string cwd = boost::filesystem::current_path().string();
    std::cout << cwd << std::endl;
    
    FolderEntryLeft[cwd] = GetFolderSubEntryCount(cwd);
    FolderStack.push_back(cwd);
    std::cout << CollectFiles(cwd) << std::endl;
    
    return 0;
}

