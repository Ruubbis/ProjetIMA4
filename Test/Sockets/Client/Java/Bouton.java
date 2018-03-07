import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;
import javax.swing.JButton;

public class Bouton extends JButton implements MouseListener{
	private String data;

	public Bouton(String str){
		this.data = str;
		this.addMouseListener(this);
	}

	public void mouseClicked(MouseEv
