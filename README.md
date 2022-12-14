## 文件结构
* src目录库存放所有c++代码
* include存放所有c++头文件
* tests文件夹存放所有JS/TS测试文件

## 框架选择

* JS/TS测试框架选择`mocha`
* c++插件选择`node-addon-api`

## 代码提交规范

* [c++:xxx]代表与c++代码相关
* [JS:xxx]代表与js代码相关
* [Dependency]添加一些依赖
* [Readme] 更新readme文档
* [Configuration] 更新项目的配置文档
* [Package]更新package.json的配置,例如`scripts`等

## 项目运行
* cmake -B build-cmake -G Ninja . &&  cd build-cmake  && ninja

./calculator "1+2+2-1"

* 用JS的办法运行测试
> 至少需要电脑上安装了Python，可以看下安装依赖过程中报啥错，然后解决掉
```shell
npm install
npm run build:addon
npm run test
```
这个就可以打印出JS的测试结果