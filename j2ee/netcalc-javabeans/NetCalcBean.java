package cloud.macca;

import java.io.Serializable;

public class NetCalcBean implements Serializable {
	
	// serve per renderla serializzabile 
	private static final long serialVersionUID = 8632260974167805909L;
	
	private String dotted_ip;
	private int cidr_netmask;
	
	public void setIP(String dotted) { this.dotted_ip = dotted; }
	public void setNetmask(int netmask) { this.cidr_netmask = netmask; } 
	public String getIP() { return this.dotted_ip;  }
	public int getNetmask() { return this.cidr_netmask; } 
	
	public int[] getDottedNetmask() {
		
		int shft = 0xffffffff << (32 - this.cidr_netmask);
		int oct1 = ((byte) ((shft&0xff000000)>>24)) & 0xff; 
		int oct2 = ((byte) ((shft&0x00ff0000)>>16)) & 0xff; 
		int oct3 = ((byte) ((shft&0x0000ff00)>>8)) & 0xff; 
		int oct4 = ((byte) (shft&0x000000ff)) & 0xff; 
		return new int[] { oct1, oct2, oct3, oct4}; 
		
	}
	
	public int[] getIntIp() throws Exception {
		String[] octs = this.dotted_ip.split("\\.");
		int[] int_ip = new int[4];
		if (octs.length !=4)
            throw new Exception("invalid ip");
		for(int i = 0; i < 4; i++) {
			int_ip[i] = Integer.parseInt(octs[i]);
			if(int_ip[i] < 0 || int_ip[i] > 255) {
				throw new Exception("invalid ip");
			}
		}
		return int_ip;
	}
	
	public int[] getNetId() throws Exception {
		int[] netid = new int[4];
	    for (int i=0; i<4; i++) {
	       netid[i] = this.getIntIp()[i] & this.getDottedNetmask()[i];
	    }
		return netid;
	}
	
	public static String stringifyIp(int[] int_ip) throws Exception {
		
		String s = "";
		for(int i = 0; i < 4; i++) {
			
			if(i == 3) s += String.valueOf(int_ip[i]);
			else s += String.valueOf(int_ip[i]) + ".";
			
		}
		
		return s;
		
	}
	
}