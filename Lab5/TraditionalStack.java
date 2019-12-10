import javax.swing.text.Element;
import java.util.*;

public class TraditionalStack <ElementType> implements Stack
{
    private List<Object> localStack;

    TraditionalStack()
    {
        localStack = new ArrayList<Object>();
    }

    public void push(Object Item)
    {
        localStack.add(Item);
    }

    @Override
    public Object pop()
    {
        return localStack.remove(localStack.size() - 1);
    }

    @Override
    public Object peek()
    {
        return localStack.get(localStack.size() - 1);
    }

    @Override
    public boolean isEmpty()
    {
        return (localStack.size() == 0);
    }
}
