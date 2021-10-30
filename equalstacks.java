import java.io.*;
import java.math.*;
import java.security.*;
import java.text.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.*;
import java.util.regex.*;
import java.util.stream.*;
import static java.util.stream.Collectors.joining;
import static java.util.stream.Collectors.toList;

class Result {

    /*
     * Complete the 'equalStacks' function below.
     *
     * The function is expected to return an INTEGER.
     * The function accepts following parameters:
     *  1. INTEGER_ARRAY h1
     *  2. INTEGER_ARRAY h2
     *  3. INTEGER_ARRAY h3
     */

    public static int equalStacks(List<Integer> h1, List<Integer> h2, List<Integer> h3) {
    // Write your code here
    Stack<Integer> s1=new Stack<Integer>();
    Stack<Integer> s2=new Stack<Integer>();
    Stack<Integer> s3=new Stack<Integer>();
    int m1=0, m2=0, m3=0;
    for(int i=h1.size()-1;i>=0;i--){
        m1+=h1.get(i);
        s1.push(m1);
    }
    for(int i=h2.size()-1;i>=0;i--){
        m2+=h2.get(i);
        s2.push(m2);
    }
    for(int i=h3.size()-1;i>=0;i--){
        m3+=h3.get(i);
        s3.push(m3);
    }
    while(true){
        if(s1.isEmpty()||s2.isEmpty()||s3.isEmpty()){
            return 0;
        }
        m1=s1.peek();
        m2=s2.peek();
        m3=s3.peek();
        if(m1==m2&&m2==m3){
            return m1;
        }
        if(m1>=m2&&m1>=m3){
            s1.pop();
        }
        if(m2>=m1&&m2>=m3){
            s2.pop();
        }
        if(m3>=m2&&m3>=m1){
            s3.pop();
        }
    }
    }

}

public class Solution {
    public static void main(String[] args) throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(System.getenv("OUTPUT_PATH")));

        String[] firstMultipleInput = bufferedReader.readLine().replaceAll("\\s+$", "").split(" ");

        int n1 = Integer.parseInt(firstMultipleInput[0]);

        int n2 = Integer.parseInt(firstMultipleInput[1]);

        int n3 = Integer.parseInt(firstMultipleInput[2]);

        List<Integer> h1 = Stream.of(bufferedReader.readLine().replaceAll("\\s+$", "").split(" "))
            .map(Integer::parseInt)
            .collect(toList());

        List<Integer> h2 = Stream.of(bufferedReader.readLine().replaceAll("\\s+$", "").split(" "))
            .map(Integer::parseInt)
            .collect(toList());

        List<Integer> h3 = Stream.of(bufferedReader.readLine().replaceAll("\\s+$", "").split(" "))
            .map(Integer::parseInt)
            .collect(toList());

        int result = Result.equalStacks(h1, h2, h3);

        bufferedWriter.write(String.valueOf(result));
        bufferedWriter.newLine();

        bufferedReader.close();
        bufferedWriter.close();
    }
}
