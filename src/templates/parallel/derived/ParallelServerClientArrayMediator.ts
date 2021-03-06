﻿/// <reference path="ParallelClientArrayMediator.ts" />

import { ParallelClientArrayMediator } from "./ParallelClientArrayMediator";
import { IExternalServerClientArray } from "../../external/interfaces/IExternalServerClientArray";

import { ExternalSystem } from "../../external/ExternalSystem";
import { ParallelSystem } from "../ParallelSystem";
import { IExternalServer } from "../../external/interfaces/IExternalServer";

import { XML } from "sxml";

/**
 * Mediator of Parallel Processing System, be a server and client at the same time as a **master**.
 *
 * The {@link ParallelServerClientArrayMediator} is an abstract class, derived from the 
 * {@link ParallelSystemArrayMediator} class, opening a server accepting {@link ParallelSystem parallel clients} and 
 * being a client connecting to {@link IParallelServer parallel servers} at the same time.
 *
 * Extends this {@link ParallelServerClientArrayMediator} and overrides below methods. After the overridings, open 
 * server with {@link open open()} method and connect to {@link IParallelServer parallel servers} through the
 * {@link connect connect()} method.
 *
 * - {@link createServerBase createServerBase()}
 * - {@link createExternalClient createExternalClient()}
 * - {@link createExternalServer createExternalServer()}
 * 
 * #### [Inherited] {@link ParallelSystemArrayMediator}
 * @copydoc ParallelClientArrayMediator
 */
export abstract class ParallelServerClientArrayMediator<System extends ParallelSystem>
	extends ParallelClientArrayMediator<System>
	implements IExternalServerClientArray<System>
{
	/* ---------------------------------------------------------
		CONSTRUCTORS
	--------------------------------------------------------- */
	/**
	 * Default Constructor.
	 */
	public constructor()
	{
		super();
	}

	/**
	 * Factory method of a child Entity.
	 *
	 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
	 *
	 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
	 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
	 */
	public createChild(xml: XML): System
	{
		return this.createExternalServer(xml);
	}

	/**
	 * Factory method creating an {@link IParallelServer} object.
	 *
	 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
	 * @return A newly created {@link IParallelServer} object.
	 */
	protected abstract createExternalServer(xml: XML): System;

	/* ---------------------------------------------------------
		METHOD OF CLIENT
	--------------------------------------------------------- */
	/**
	 * @inheritdoc
	 */
	public connect(): void
	{
		for (let i: number = 0; i < this.size(); i++)
		{
			let system: ExternalSystem = this.at(i);
			if ((system as IExternalServer).connect == undefined)
				continue;

			(system as IExternalServer).connect();
		}
	}
}