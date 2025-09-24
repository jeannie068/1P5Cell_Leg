#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // 定義參數
    const int die_height = 342000;  
    const int row_spacing = 1000;   // 每行間距

    const string output_filename = "double_row_def.txt";
    
    // 開啟輸出檔案
    ofstream outfile(output_filename);
    if (!outfile.is_open()) {
        cerr << "無法開啟檔案: " << output_filename << endl;
        return 1;
    }
    
    // 生成ROW行
    int row_count = 0;
    for (int y_pos = 0; y_pos < die_height; y_pos += row_spacing) {
        // 決定方向：偶數行用N，奇數行用FS
        string orientation = (row_count % 2 == 0) ? "N" : "FS";
        
        // 生成ROW行
        outfile << "ROW core_SITE_ROW_" << row_count 
                << " core 0 " << y_pos << " " << orientation 
                << " DO 1710 BY 1 STEP 200 0 ;" << endl;
        
        row_count++;
        
        // 防止超過die_height
        if (y_pos == die_height) break;
    }
    
    // 關閉檔案
    outfile.close();
    
    cout << "成功生成 " << row_count << " 行ROW資料到檔案: " << output_filename << endl;
    cout << "範圍: Y座標從 0 到 " << die_height << endl;
    
    return 0;
}