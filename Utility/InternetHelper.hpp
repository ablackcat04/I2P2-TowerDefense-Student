//
// Created by wuru8 on 2024/6/13.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_INTERNETHELPER_HPP
#define INC_2024_I2P2_TOWERDEFENSE_INTERNETHELPER_HPP

#include <iostream>
#include <fstream>

namespace InternetHelper {
    const std::string server_ip = "http://192.168.56.1";
    const std::string upload_php_position = server_ip + "/I2P_project/upload.php";

    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

    bool downloadFile(const std::string &url, const std::string &localPath);

    bool uploadFile(const std::string &url, const std::string &localPath);
};


#endif //INC_2024_I2P2_TOWERDEFENSE_INTERNETHELPER_HPP
