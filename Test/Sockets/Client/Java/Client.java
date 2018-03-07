import java.io.IOException;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;


public class Client{
	public static void main(String[] argv){
		Fenetre win = new Fenetre();		
		/*
		Socket socket;
		PrintWriter out;
		Scanner sc = new Scanner(System.in);
		String str;
		boolean boucle = true;

		try{
			System.out.println("Tentative de connexion");
			socket = new Socket(InetAddress.getLocalHost(),9000);
			System.out.println("Connexion etablie");
			out = new PrintWriter(socket.getOutputStream());
			while(boucle){
				str = sc.nextLine();
				if(str != "stop"){
					out.println(str);
					out.flush();
				}
				else{
					boucle = false;
				}
			}
			System.out.println("Fermeture Connexion");
			socket.close();
		}
		catch (UnknownHostException e){
			e.printStackTrace();
		}
		catch (IOException e){
			e.printStackTrace();
		}
		
*/
	}
}
