/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:50:04 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/08 21:25:21 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct File
{
    bool isValid;

    std::string name;
    std::string extention;
    std::vector<std::string> contents;
};

class FileManager
{
public:
    FileManager();
    FileManager(const FileManager &other);
    ~FileManager();
    FileManager &operator=(const FileManager &other);

    static File openFile(std::string filename);
    static void printFileInfo(File file);
    static void printFileContents(File file);

private:
};

#endif