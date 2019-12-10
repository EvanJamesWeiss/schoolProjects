import javax.swing.*;
import java.awt.*;
public class StackApp extends JFrame {
	public StackApp(String title) {
		super(title);
		this.setSize(600, 450);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		StackPanel stackPanel = new StackPanel();
		LocationHolder holder = new LocationHolder();
		JPanel buttonPanel = new JPanel (new FlowLayout());
		buttonPanel.add(new StackPushButton(stackPanel,holder));
		buttonPanel.add(new StackPopButton(stackPanel,holder));
		buttonPanel.add(new StackPeekButton(stackPanel,holder));
		this.add(stackPanel, BorderLayout.CENTER);
		this.add(buttonPanel, BorderLayout.SOUTH);
		this.setVisible(true);
	}
	public static void main (String [ ] args) {
		StackApp app = new StackApp ("Stack Implementation with Java Generics");
	}
}
