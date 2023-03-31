import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter; 
import java.io.FileWriter; 
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

/*
alat kalo mau ganti background

CARA PENGGUNAAN

- ganti nama file buffered image dengan gambar aspek rasio harus hasil kali integer dari 320x200 terus set mod sama leninency

- mod: pembagi resolusi, kalo resolusinya lebih gede dari 320x200 harus dibagi sampe demikian (contoh 1280x800, jadi mod = 4 biar hasilnya 320x200)
- leninency: akurasi warna, semakin kecil leninency semakin banyak jumlah warna yang disimpen, warna gak boleh lebih dari 256, kalo array colors index out of range artinya leninency perlu ditinggin
- hasil: result.txt, isinya bakalan 2 angka angka ngumpul
         angka yang di atas itu bitmap warna, copy ke stdbg.h yang bagian background
         angka yang di bawah itu palette yang perlu, tapi 0 yang di bagian atas itu gak perlu, jadi copy ke graphics.c yang bagian palette yang nilai gak nol di bawah, pastiin jumlah warna palettenya tetep 256
*/

public class ImageConverter {

    public static void main(String[] args) {
        try{
            BufferedImage image = ImageIO.read(new File("waifu4.jpg"));     //kalo mau ganti nama file di sini
            
            int mod = 4;            //mod yang udah disebut
            int leninency = 4;      //leninency yang udah disebut

            int width = image.getWidth();
            int height = image.getHeight();

            int reswidth = width / mod;
            int resheight = height / mod;

            int colorCombined = reswidth * 3;
    
            int[][] pixels = new int[resheight][colorCombined];

            int[][] buffer = new int[200][320];

            int colorcount = 0;
            int[][] colors = new int[256][3];
    
            for(int y = 0; y < resheight; y++){
                for(int x = 0; x < reswidth; x++){
                    // baca gambar per pixel
                    Color color = new Color(image.getRGB(x * mod,y * mod));
                    
                    int loc = x*3;

                    // convert ke 18 bit color per value
                    pixels[y][loc] = (color.getRed() * 63) / 255;
                    pixels[y][loc + 1] = (color.getGreen() * 63) / 255;
                    pixels[y][loc + 2] = (color.getBlue() * 63) / 255;
                    
                    if(colorcount == 0){
                        // inisiasi palette
                        colors[colorcount][0] = pixels[y][loc];
                        colors[colorcount][1] = pixels[y][loc + 1];
                        colors[colorcount][2] = pixels[y][loc + 2];
                        colorcount++;
                    }
                    else{
                        boolean found = false;
                        int i = 0;
                        for(i = 0; i < colorcount; i++){
                            // nyari di palette yang udah ada
                            if((colors[i][0] - leninency <= pixels[y][loc]  && colors[i][0] + leninency >= pixels[y][loc]) &&
                                (colors[i][1] - leninency <= pixels[y][loc + 1] && colors[i][1] + leninency >= pixels[y][loc + 1]) &&
                                (colors[i][2] - leninency <= pixels[y][loc + 2] && colors[i][2] + leninency >= pixels[y][loc + 2]))
                            {
                                found = true;
                                break;
                            }
                        }

                        if(!found){
                            // nambah palette
                            colors[colorcount][0] = pixels[y][loc];
                            colors[colorcount][1] = pixels[y][loc + 1];
                            colors[colorcount][2] = pixels[y][loc + 2];
                            buffer[y][x] = colorcount;

                            colorcount++;
                        }
                        else{
                            // ngisi bitmap
                            buffer[y][x] = i;
                        }
                    }
                    
                }
            }

            BufferedWriter bw = new BufferedWriter(new FileWriter("result.txt"));   //hasilnya ada di result.txt

            // nulis bitmap
            for(int y = 0; y < 200; y++){
                for(int x = 0; x < 320; x++){
                    bw.write((255 - buffer[y][x]) + ((x == 320 - 1 && y == 200 - 1) ? "" : ", "));
                }
                bw.newLine();
            }
            bw.newLine();
            bw.newLine();
            bw.newLine();

            // nulis palette
            for(int y = 255; y >= 0; y--){
                if ((255 - y) % 4 == 0){
                    bw.newLine();
                }
                for(int x = 0; x < 3; x++){
                    bw.write(colors[y][x] + ((x == 2 && y == 0) ? "" : ", "));
                }
            }

            bw.flush();
            bw.close();

        } catch(IOException e){
            System.out.println("Error: " + e);
        }
    }
}
