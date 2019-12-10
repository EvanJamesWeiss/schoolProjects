import javax.swing.*;
import java.awt.*;
public class StackPanel extends JPanel
{   private Stack<SmartRectangle> _stack;
    private SmartRectangle _displayRect;

    StackPanel() {
        _stack = new TraditionalStack<SmartRectangle>();
        int SQUARE_SIZE = 20;
        _displayRect = new SmartRectangle(50, 50, SQUARE_SIZE, SQUARE_SIZE, this.getBackground(), this);
    }

    Stack<SmartRectangle> getStack() {
       return _stack;
    }

    SmartRectangle getDisplayRect() {
        return _displayRect;
    }

    public void paintComponent(Graphics aBrush) {
           super.paintComponent(aBrush);
            Graphics2D aBetterBrush = (Graphics2D) aBrush;
            Stack<SmartRectangle> tempStack = new TraditionalStack<SmartRectangle>();
                _displayRect.fill(aBetterBrush);
                while (!_stack.isEmpty()) {
                    //remove+display elements
                    SmartRectangle rectangle = _stack.pop();
                    if (rectangle != null)
                    {
                        rectangle.fill(aBetterBrush);
                    }
                    tempStack.push(rectangle);
                }
                while (!tempStack.isEmpty()) // put elements back
                    _stack.push(tempStack.pop());
                _displayRect.setColor(this.getBackground());
        }
    }
