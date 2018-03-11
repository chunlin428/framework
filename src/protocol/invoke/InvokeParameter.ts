﻿import { XML } from "sxml";
import { Entity } from "../entity/Entity";

/**
 * A parameter belongs to an Invoke.
 * 
 * ![Class Diagram](http://samchon.github.io/framework/images/design/ts_class_diagram/protocol_message_protocol.png)
 * 
 * @author Jeongho Nam <http://samchon.org>
 */
export class InvokeParameter
	extends Entity
{
	/**
	 * Name of the parameter.
	 *
	 * @details Optional property, can be omitted.
	 */
	protected name: string = "";

	/**
	 * Type of the parameter.
	 */
	protected type: string = "";

	/** 
	 * Value of the parameter.
	 */
	protected value: boolean | number | string | XML | Uint8Array = null;

	/**
	 * Default Constructor.
	 */
	public constructor();

	public constructor(val: boolean);
	public constructor(val: number);
	public constructor(val: string);
	public constructor(val: XML);
	public constructor(val: Uint8Array);

	/**
	 * Construct from variable name and number value.
	 * 
	 * @param name
	 * @param val
	 */
	public constructor(name: string, val: boolean);
	public constructor(name: string, val: number);
	public constructor(name: string, val: string);
	public constructor(name: string, val: XML);
	public constructor(name: string, val: Uint8Array);
	
	/* -------------------------------------------------------------------
		CONSTRUCTORS
	------------------------------------------------------------------- */
	public constructor(...args: any[])
	{
		super();

		// DEFAULT CONSTRUCTOR
		if (args.length == 0)
			return;

		// INITIALIZATION CONSTRUCTOR
		if (args.length == 1)
		{
			this.name = "";
			this.setValue(args[0]);
		}
		else
		{
			this.name = args[0];
			this.setValue(args[1]);
		}
	}

	/**
	 * @inheritdoc
	 */
	public construct(xml:  XML): void
	{
		this.name = (xml.hasProperty("name")) ? xml.getProperty("name") : "";
		this.type = xml.getProperty("type");

		if (this.type == "XML")
			this.value = xml.begin().second.front();
		else if (this.type == "boolean")
		{
			let value: string = xml.getValue();
			this.value = (value != "0" && value != "false"); // Do not use the Boolean(value);
		}
		else if (this.type == "number")
			this.value = Number(xml.getValue());
		else if (this.type == "string")
			this.value = xml.getValue();
	}

	public setValue(value: boolean): void;
	public setValue(value: number): void;
	public setValue(value: string): void;
	public setValue(value: XML): void;
	public setValue(value: Uint8Array): void;

	public setValue(value: boolean | number | string | XML | Uint8Array): void
	{
		this.value = value;

		if (value instanceof XML)
			this.type = "XML";
		else if (value instanceof Uint8Array)
			this.type = "ByteArray";
		else
			this.type = typeof value;
	}

	/* -------------------------------------------------------------------
		GETTERS
	------------------------------------------------------------------- */
	/**
	 * @inheritdoc
	 */
	public key(): any
	{
		return this.name;
	}

	/**
	 * Get name.
	 */
	public getName(): string
	{
		return this.name;
	}

	/**
	 * Get type.
	 */
	public getType(): string
	{
		return this.type;
	}

	/**
	 * Get value.
	 */
	public getValue(): any
	{
		return this.value;
	}

	/* -------------------------------------------------------------------
		EXPORTERS
	------------------------------------------------------------------- */
	/**
	 * @inheritdoc
	 */
	public TAG(): string
	{
		return "parameter";
	}
	
	/**
	 * @inheritdoc
	 */
	public toXML(): XML
	{
		let xml: XML = new XML();
		xml.setTag(this.TAG());

		if (this.name != "")
			xml.setProperty("name", this.name);
		xml.setProperty("type", this.type);

		// NOT CONSIDERED ABOUT THE BINARY DATA
		if (this.type == "XML")
			xml.push(this.value as XML);
		else if (this.type != "ByteArray")
			xml.setValue(this.value + "");

		return xml;
	}
}