import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;

public class Client{
	public static void main(String[] str){
		
		Socket socket;

		try{
			System.out.println("Tentative de connexion");
			socket = new Socket(InetAddress.getLocalHost(),9000);
			System.out.println("Connexion etablie");
			System.out.println("Fermeture Connexion");
			socket.close();
		}
		catch (UnknownHostException e){
			e.printStackTrace();
		}
		catch (IOException e){
			e.printStackTrace();
		}

	}
}
