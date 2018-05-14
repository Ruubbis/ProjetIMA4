
import java.io.IOException;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;

import java.awt.Graphics;
import java.awt.Dimension;
import java.awt.Color;
import java.awt.Insets;
import java.awt.GridLayout;
import java.awt.Font;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JRadioButton;
import javax.swing.ButtonGroup;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

 
public class Fenetre extends JFrame{
	private JButton buttonAllumer = new JButton("Allumer la lumiere");
	private JButton buttonEteindre = new JButton("Eteindre la lumiere");
	private JButton buttonEnvoieMsg = new JButton("Envoyer le message");
	private JPanel panel = new JPanel();
	private JLabel title = new JLabel("Controleur Xbee USB");
	private JTextField jtf = new JTextField("Entrez votre message");

	private MatrixButton[][] bmatrix = new MatrixButton[64][32];
	
	ButtonGroup bg = new ButtonGroup();
	private JRadioButton jrBlanc = new JRadioButton("Blanc");
	private JRadioButton jrRouge = 	new JRadioButton("Rouge");
	private JRadioButton jrBleu = new JRadioButton("Bleu");
	private JRadioButton jrJaune = new JRadioButton("Jaune");
	private JRadioButton jrNoir = new JRadioButton("Noir");
	
	Font police = new Font("Arial",Font.BOLD, 14);

	Dimension windowSize = new Dimension(1100,900);
	Dimension size = new Dimension(400,80);

	private JPanel matrix = new JPanel();

	private Socket socket;
	private PrintWriter out;
	private String str;

	public Fenetre(){

		this.setTitle("Controller Xbee");
        	this.setSize(windowSize);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null);
		
		Insets insets = panel.getInsets();

		buttonAllumer.addActionListener(new A1Listener());
		buttonEteindre.addActionListener(new A0Listener());
		buttonEnvoieMsg.addActionListener(new B1Listener());
				
		panel.setLayout(null);
		panel.add(buttonAllumer);
		panel.add(buttonEteindre);
		panel.add(buttonEnvoieMsg);
		panel.add(title);
		panel.add(jtf);

		matrix.setLayout(new GridLayout(64,32));


		title.setBounds(insets.left + 500, insets.top + 5, 600, 80);
		buttonAllumer.setBounds(insets.left + 60, insets.top + 70, size.width, size.height);
		buttonEteindre.setBounds(insets.left + (windowSize.width - size.width - 60), insets.top + 70, size.width, size.height);
		buttonEnvoieMsg.setBounds(insets.left + (windowSize.width - size.width - 60), insets.top + 180, size.width-80, size.height);
		jtf.setBounds(insets.left + 60, insets.top + 180, size.width+80, size.height);

		jrNoir.setSelected(true);
		jrBlanc.setFont(police);
		jrRouge.setFont(police);
		jrBleu.setFont(police);
		jrJaune.setFont(police);
		jrNoir.setFont(police);
		panel.add(jrBlanc);
		panel.add(jrRouge);
		panel.add(jrBleu);
		panel.add(jrJaune);
		panel.add(jrNoir);

		bg.add(jrBlanc);
		bg.add(jrRouge);
		bg.add(jrBleu);
		bg.add(jrJaune);
		bg.add(jrNoir);



		jrBlanc.setBounds(insets.left + 160, insets.top + 280, 80,20);
		jrRouge.setBounds(insets.left + 320, insets.top + 280, 80,20);
		jrBleu.setBounds(insets.left + 480, insets.top + 280, 80,20);
		jrJaune.setBounds(insets.left + 640, insets.top + 280, 80,20);
		jrNoir.setBounds(insets.left + 800, insets.top + 280, 80,20);

		for(int j=0;j<32;j++){
			for(int i=0;i<64;i++){
				bmatrix[i][j] = new MatrixButton("");
				bmatrix[i][j].addActionListener(new BMListener());
				bmatrix[i][j].posx = i;
				bmatrix[i][j].posy = j;
				panel.add(bmatrix[i][j]);
				bmatrix[i][j].setBounds(insets.left+38+i*16, insets.top + 320 +j*16,16,16);
			}
		}
		

		
		this.setContentPane(panel);
		this.setVisible(true);

		socketClient();
	}

	private void socketClient(){
		try{
			socket = new Socket(InetAddress.getLocalHost(),9000);
			out = new PrintWriter(socket.getOutputStream());		
		}		
		catch (UnknownHostException e){
			System.out.println("Impossible de trouver le server");
		}
		catch (IOException e){
			System.out.println("Impossible de trouver le serveur");
			System.exit(0);
		}
	}

	class A1Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			out.println("[A-1]");
			out.flush();
		}
	}

	class A0Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			out.println("[A-0]");
			out.flush();
		}
	}

	class B1Listener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			String msg = jtf.getText();
			out.println("[B-"+msg+"]");
			out.flush();
		}
	}

	class BMListener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			Object src = e.getSource();
			if(src instanceof MatrixButton){
				MatrixButton button = (MatrixButton) src;
				if(jrBlanc.isSelected()){
					button.setBackground(Color.WHITE);
					out.println("[C-"+button.posx+" "+button.posy+" 0]");
					out.flush();
				}
				else if(jrRouge.isSelected()){
					button.setBackground(Color.RED);
					out.println("[C-"+button.posx+" "+button.posy+" 1]");
					out.flush();
				}
				else if(jrBleu.isSelected()){
					button.setBackground(Color.BLUE);
					out.println("[C-"+button.posx+" "+button.posy+" 2]");
					out.flush();
				}
				else if(jrJaune.isSelected()){
					button.setBackground(Color.YELLOW);
					out.println("[C-"+button.posx+" "+button.posy+" 3]");
					out.flush();
				}
				else{
					button.setBackground(Color.BLACK);
					out.println("[C-"+button.posx+" "+button.posy+" 4]");
					out.flush();
				}
			}
		}
	}
	
}	
