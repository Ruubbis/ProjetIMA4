
import java.io.IOException;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;

import java.awt.GridLayout; 
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

 
public class Fenetre extends JFrame{
	private JButton A_on = new JButton("Allumer A");
	private JButton B_on = new JButton("Allumer B");
	private JButton A_off = new JButton("Eteindre A");
	private JButton B_off = new JButton("Eteindre B");
	private JPanel panel = new JPanel();
	
	private Socket socket;
	private PrintWriter out;
	private String str;

	public Fenetre(){
		this.setTitle("Controller Xbee");
        	this.setSize(300, 300);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null);
		

		A_on.addActionListener(new A1Listener());
		A_off.addActionListener(new A0Listener());
		B_on.addActionListener(new B1Listener());
		B_off.addActionListener(new B0Listener());
				

		panel.setLayout(new GridLayout(2, 2));
		panel.add(A_on);
		panel.add(B_on);
		panel.add(A_off);
		panel.add(B_off);

		this.setContentPane(panel);
		this.setVisible(true);

		socketClient();
	}

	private void socketClient(){
		try{
			System.out.println("Tentative de connexion");
			socket = new Socket(InetAddress.getLocalHost(),9000);
			System.out.println("Connexion etablie");
			out = new PrintWriter(socket.getOutputStream());		
		}		
		catch (UnknownHostException e){
			e.printStackTrace();
		}
		catch (IOException e){
			e.printStackTrace();
		}
	}

	class A1Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			out.println("A-1");
			out.flush();
		}
	}

	class A0Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			out.println("A-0");
			out.flush();
		}
	}

	class B1Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			out.println("B-1");
			out.flush();
		}
	}

	class B0Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			out.println("B-0");
			out.flush();
		}
	}

}	
