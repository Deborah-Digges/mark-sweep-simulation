import java.lang.management.ManagementFactory;
import java.lang.management.MemoryMXBean;
import java.lang.management.MemoryNotificationInfo;
import java.lang.management.MemoryPoolMXBean;
import java.lang.management.MemoryType;
import java.lang.management.MemoryUsage;
import java.util.ArrayList;
import java.util.List;

import javax.management.Notification;
import javax.management.NotificationEmitter;
import javax.management.NotificationListener;

public class MemoryTest {
    
	public static void main(String[] args)
	{
		startDemonstration();
        }
	
	public static void startDemonstration()
	{
		        	
		StringBuffer buffer = new StringBuffer();
		long t0 = 0;
		int capacity = 1000000;
		List<String> strings = new ArrayList<String>(capacity);

		
		/* Build a really big string */
		for (int i = 0; i < 1000; i++)
		{
			buffer.append("This is an unneccessarily long string...But you know why\n");
        	}
        

		for (int i = 0; i < capacity ; i++)
		{
			/* increase memory usage */
			strings.add("randomString:" + Math.random() + buffer);

          		/* Generate some garbage */
			for (int j = 0; j < 100; j++)
			{
				strings.set((int)Math.floor(Math.random() * strings.size()),
								"randomString:" + Math.random() + buffer);
			}
	 
			/* Print the memory usage every now and then */
			if (i%100 == 0)
			{
				long t1 = System.currentTimeMillis();
				if (t1-t0 > 100)
				{
                    			t0 = t1;
                    			System.out.println("it=" + pad(String.valueOf(i), 4) + memString());
                		}
			}
		}
	}

	public static String pad(String str, int i)
	{
		while (str.length()<i)	
		{
			str = " " + str;
		}
		return str;
	}
    

	private static String memString()
	{
	
		StringBuffer buffer = new StringBuffer();
		for (MemoryPoolMXBean pool : ManagementFactory.getMemoryPoolMXBeans())
		{
			if (pool.getType().equals(MemoryType.HEAP))
			{
				buffer.append(" - ")
				.append(pool.getName())
				/* usage includes unreachable objects that are not collected yet */
				.append(": u=").append(toString(pool.getUsage()))
				/* usage as of right after the last collection */
				.append(" cu=").append(toString(pool.getCollectionUsage()));
		      
			}
		}   
		return buffer.toString();
	}

	private static String toString(MemoryUsage memoryUsage)
	{
		String string = (memoryUsage.getUsed() * 100) / memoryUsage.getMax() + "%";
		return pad(string, 4);
	}

}
