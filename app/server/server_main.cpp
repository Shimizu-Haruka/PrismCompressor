#include"httplib.h"
#include"prism/core/compression_engine.h"
#include<iostream>
#include<vector>
#include<string>

int main()
{
    httplib::Server svr;
    svr.Get("/hello" , [](const httplib::Request& req ,httplib::Response& res){
        res.set_content("hello cpp." , "text/plain");
    });

    svr.set_default_headers({//配置跨域请求
        {"Access-Control-Allow-Origin" , "*"},//允许任何来源（*）访问
        {"Access-Control-Allow-Methods" , "POST GET OPTIONS"},//允许前端用什么方法请求
        {"Access-Control-Allow-Headers" , "Content-Type"},//允许前端在请求头里携带 Content-Type
        {"Access-Control-Expose-Headers" , "X-Original-Size, X-Compressed-Size"}
    });

    //配置预检请求
    svr.Options(R"(.*)"/*Raw String 原始字符串*/ , /*lambda匿名函数*/[](const httplib::Request& , httplib::Response& res){
        res.status = 200;//设置响应状态码为200 放行
    });//路由 拦截http的options请求

    svr.Post("/compress" , [](const httplib::Request& req , httplib::Response& res){
        if(req.form.files.count("file") == 0){//无文件返回错误 400
            res.status = 400;
            res.set_content("Error : Missing file payload" , "text/plain");
            return;
        }

        auto it = req.form.files.find("file");//安全提取参数
        auto file = it->second;

        if(file.content.empty())
        {
            res.status = 400;// 400 Bad request
            res.set_content("Error: Empty file." , "text/plain");
            return;
        }

        std::string algo = req.get_param_value("algo");//get_param_value 提取 GET 请求的查询参数
        prism::CompressionOptions options;
        if(algo == "RLE") options.algorithm = prism::AlgorithmType::RLE;
        else if(algo == "Huffman") options.algorithm = prism::AlgorithmType::Huffman;
        else if(algo == "LZ77") options.algorithm = prism::AlgorithmType::LZ77;

        prism::ByteBuffer input(file.content.begin() , file.content.end());//string转化为ByteBuffer

        prism::core::CompressionEngine engine;
        auto result = engine.compressBuffer(input , options);//实例化引擎并进行压缩

        if(result.data.empty())
        {
            res.status = 500;//报错
            res.set_content("Error: Compression failed." , "text/plain");
            return;
        }

        //统计信息放入信息头
        res.set_header("X-Original-Size" , std::to_string(result.stats.originalSize));
        res.set_header("X-Compressed-Size" , std::to_string(result.stats.compressedSize));

        //二进制数据作为响应体
        res.set_content(
            // reinterpret_cast 是为了把 uint8_t* 转成 char*，因为 set_content 需要 char*
            reinterpret_cast<const char*>/*强制类型转换*/(result.data.data()), result.data.size() , "application/octet-stream"//纯二进制字节流
        );
    });

    svr.Post("/decompress" , [](const httplib::Request& req , httplib::Response& res){
        if(req.form.files.count("file") == 0){
            res.status = 400;
            res.set_content("Error: Missing file payload" , "text/plain");
            return;
        }

        auto it = req.form.files.find("file");
        auto file = it->second;

        if(file.content.empty())
        {
            res.status = 400;
            res.set_content("Error: Empty file." , "text/plain");
            return;
        }

        prism::ByteBuffer input(file.content.begin() , file.content.end());

        prism::core::CompressionEngine engine;
        prism::ByteBuffer result = engine.decompressBuffer(input);

        if (result.empty()) {
            res.status = 500;
            res.set_content("Error: Decompression failed.", "text/plain");
            return;
        }

        //把原始文件作为二进制返回
        res.set_content(
            reinterpret_cast<const char*>(result.data()),
            result.size(),
            "application/octet-stream"
        );
    });

    std::cout << "open http://localhost:8080/" << std::endl;
    svr.listen("0.0.0.0" , 8080);
}