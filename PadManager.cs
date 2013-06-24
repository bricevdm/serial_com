/*************************************************************************
* File Name          : PadManager.cs
* Author             : Brice & Fred
* Updated            : van
* Version            : V1.0.0
* Date               : 6/24/2013
* Description        : Process Xbox Pad Input and output to serial
* License            : CC-BY-SA 3.0
**************************************************************************/

using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class PadManager : MonoBehaviour {
  
	public bool outputToSerial;
	public GUIText log;
	public float delta;
	
	private int previousArmAngle = 90;
	private int currentArmAngle = 90;
	public int armSpeed = 2;
	public int armMinAngle = 0;
	public int armMaxAngle = 180;

	public int clawSpeed = 2;
	public int clawMinAngle = 0;
	public int clawMaxAngle = 180;

	public static SerialPort sp = new SerialPort("\\\\.\\COM6", 9600);//"USBSER000", 9600);
	
	void Start () {

		foreach( string s in SerialPort.GetPortNames() )
		{
			Debug.Log( "port name: " + s);
		}
		
		if (!outputToSerial) return;
		
		try
		{
	    	sp.Open();
		}
		catch (System.Exception ex)
		{
			Debug.LogWarning(ex);
			log.text = ex.ToString();
		}
	}
	
	void Update () {
		Move ();
		
		/*
		if (sp.IsOpen ){
			try {
				Debug.Log(sp.ReadByte());
			}
			catch (System.Exception ex)
			{
				Debug.Log(ex);
			}
		}*/
	}
		
	void Move () {
	
		float lx = Input.GetAxis("L_XAxis_1");
		float ly = Input.GetAxis("L_YAxis_1");
		//float rx = Input.GetAxis("R_XAxis_1");
		float ry = Input.GetAxis("R_YAxis_1");
		
		float cly = -(-lx + ly)*0.67f;
		float cry = -( lx + ly)*0.67f;
		
		byte bly = (byte)( cly*127 + 127);
		byte bry = (byte)( cry*127 + 127);	
		
		if ( Mathf.Abs(lx) > delta || Mathf.Abs(ly) > delta )
			SendToSerial(1, bly, bry);
		else
			SendToSerial(2);
		
		if (Input.GetButtonDown("A_1"))
		{
			currentArmAngle = armMinAngle;
		}
		if (Input.GetButtonDown("B_1"))
		{
			currentArmAngle = armMaxAngle;
		}
		if (Input.GetButtonDown("X_1"))
		{
			SendToSerial(3, (byte) clawMinAngle);
		}
		if (Input.GetButtonDown("Y_1"))
		{
			SendToSerial(3, (byte) clawMaxAngle);
		}
		
		currentArmAngle += (int) (ry * armSpeed);
		
		currentArmAngle = Mathf.Clamp( currentArmAngle, armMinAngle, armMaxAngle);
		
		if (previousArmAngle != currentArmAngle)
		{
			SendToSerial(4, (byte) currentArmAngle);
		}
		
		previousArmAngle = currentArmAngle;
		
	}
	
	void SendToSerial(params byte[] cmd)
	{
		
		if (outputToSerial) 
		{
			if (sp.IsOpen){
		    	try { 
					sp.Write( cmd, 0, cmd.Length );
				}
		    	catch (System.Exception ex) 
				{
					Debug.LogWarning(ex);
				}
		    }
		}
		
	}
}
