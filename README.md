# Bài tập lớn môn Lý Thuyết Mật Mã
## ***Triển khai thuật toán mã dòng đối xứng HC128 họ eStream sử dụng ngôn ngữ C***

### Mô tả

Nội dung bài tập lớn bao gồm:

    1. Cơ sở lý thuyết mật mã dòng HC128 (ppt đi kèm)

    2. Triển khai thuật toán sử dụng ngôn ngữ lập trình C

    3. Thực hiện kiểm thử bằng công cụ kiểm thử của NIST


### Hướng dẫn build và test thuật toán

Đảm bảo trình biên dịch C gcc đã được cài đặt trên máy. Thực hiện các bước sau:

Bước 1: Tạo dòng khóa
```
make build
```


Bước 2: Gen key stream 
```
./output/hc128_generators 
```

Sau khi đã thực hiện câu lệnh, file data.txt chứa dòng khóa sẽ được tạo ra và lưu trong thư mục ./output (độ dài dòng khóa là 48000 bits)

Bước 3: Thực hiện kiểm thử

Sau khi kiểm thử, file report được lưu tại  ./sts-2.1.2/experiments/finalAnalysisReport.txt

File kiểm thử cho thấy dòng khóa đã tạo phía trên khá tốt 

