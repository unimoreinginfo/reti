<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">

<title>Esame 17/07/2019</title>

</head>
<body>

<%@ page import="java.nio.*" %>
<%@ page import="java.security.*" %>

<%!
	public static int convertIP(String ip) {
		String[] octects = ip.split("\\.");
		
		if (octects.length != 4) {
			throw new RuntimeException("Invalid IP: invalid octects number");	
		}
		
		int ipAddr = 0;
		for (int i = 0; i < 4; i++) {
			int octectDec = Integer.parseInt(octects[i]);
			if (octectDec < 0 || octectDec > 255) {
				throw new RuntimeException("Invalid IP: invalid octect");	
			}
			ipAddr |= octectDec << ((3 - i) * 8);
		}
		return ipAddr;
	}

	public static int convertNetmask(String cidrNetmask) {
		int cidrNetmaskInt = Integer.parseInt(cidrNetmask);
		
		if (cidrNetmaskInt <= 0 || cidrNetmaskInt > 32) {
			throw new RuntimeException("Invalid CIDR netmask");
		}
		
		int netmask = 0;
		for (int i = 0; i < cidrNetmaskInt; i++) {
			netmask |= (1 << (31 - i));
		}
		
		return netmask;
	}
	
	public static int hashIP(int ipAddr, int netmask) {
		int netId = ipAddr & netmask;
		
		int hostId = ipAddr & (~netmask);
		byte[] hostIdBytes = ByteBuffer.allocate(4).putInt(hostId).array();
		
		MessageDigest md;
		try {
			md = MessageDigest.getInstance("MD5");
		} catch (NoSuchAlgorithmException e) {
			// Shouldn't go there
			throw new RuntimeException(e);
		}
		byte[] hostIdDigest = md.digest(hostIdBytes);
		
		int hashedHostId = ByteBuffer.wrap(hostIdDigest).getInt() & (~netmask);
		
		System.out.println("hostID: " + hostId + " hashedHostId: " + hashedHostId);
		
		int res = netId | hashedHostId;
		return res;
	}
	
	public static String ipToStr(int ipAddr) {
		return String.format("%d.%d.%d.%d",
		         (ipAddr >> 24 & 0xff),         
		         (ipAddr >> 16 & 0xff),  
		         (ipAddr >> 8 & 0xff),   
		         (ipAddr & 0xff)  
		         );
	}
%>

<br>

<%
	String ipStr = request.getParameter("ip");
	String netmaskStr = request.getParameter("netmask");
%>

<!--  Input -->
<form>
IP: <input type="text" name="ip" value="<%= ipStr %>"><br>
Netmask: <input type="text" name="netmask" value=<%= netmaskStr %>><br>
<button>Submit</button>
</form>


<!--  Output -->
<%
	if (ipStr != null && netmaskStr != null) {
		try {
			int ip = convertIP(ipStr);
			int netmask = convertNetmask(netmaskStr);

			int hashedIp = hashIP(ip, netmask);
			
			out.println("IP: " + ipToStr(ip) + "<br>");
			out.println("Netmask: " + ipToStr(netmask) + "<br>");
			out.println("Hashed IP: " + ipToStr(hashedIp) + "<br>");
			
		} catch (Exception e) {
			out.println(String.format("<h1>ERROR: %s</h1>", e.getMessage()));
		}
	}
%>

</body>
</html>
