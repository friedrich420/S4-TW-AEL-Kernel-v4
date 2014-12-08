 * @return {Resource}
 */
Resource.forObject = function(obj)
{
    if (!obj)
        return null;
    if (obj instanceof Resource)
        return obj;
    if (typeof obj === "object")
        return obj["__resourceObject"];
    return null;
}

/**
 * @param {Resource|*} obj
 * @return {*}
 */
Resource.wrappedObject = function(obj)
{
    var resource = Resource.forObject(obj);
    return resource ? resource.wrappedObject() : obj;
}

/**
 * @param {Resource|*} obj
 * @param {!Cache} cache
 * @return {ReplayableResource|*}
 */
Resource.toReplayable = function(obj, cache)
{
    var resource = Resource.forObject(obj);
    return resource ? resource.toReplayable(cache) : obj;
}

Resource.prototype = {
    /**
     * @return {number}
     */
    id: function()
    {
        return this._id;
    },

    /**
     * @return {Object}
     */
    wrappedObject: function()
    {
        return this._wrappedObject;
    },

    /**
     * @param {!Object} value
     */
    setWrappedObject: function(value)
    {
        console.assert(value, "wrappedObject should not be NULL");
        console.assert(!(value instanceof Resource), "Binding a Resource object to another Resource object?");
        this._wrappedObject = value;
        this._bindObjectToResource(value);
    },

    /**
     * @return {Object}
     */
    proxyObject: function()
    {
        if (!this._proxyObject)
            this._proxyObject = this._wrapObject();
        return this._proxyObject;
    },

    /**
     * @return {ResourceTrackingManager}
     */
    manager: function()
    {
        return this._resourceManager;
    },

    /**
     * @param {ResourceTrackingManager} value
     */
    setManager: function(value)
    {
        this._resourceManager = value;
    },

    /**
     * @return {!Array.<Call>}
     */
    calls: function()
    {
        return this._calls;
    },

    /**
     * @return {ContextResource}
     */
    contextResource: function()
    {
        if (this instanceof ContextResource)
            return /** @type {ContextResource} */ (this);

        if (this._calculatingContextResource)
            return null;

        this._calculatingContextResource = true;
        var result = null;
        for (var i = 0, n = this._calls.length; i < n; ++i) {
            result = this._calls[i].resource().contextResource();
            if (result)
                break;
        }
        delete this._calculatingContextResource;
        console.assert(result, "Failed to find context resource for " + this._name + "@" + this._kindId);
        return result;
    },

    /**
     * @return {string}
     */
    toDataURL: function()
    {
        return "";
    },

    /**
     * @param {!Cache} cache
     * @return {!ReplayableResource}
     */
    toReplayable: function(cache)
    {
        var result = /** @type {ReplayableResource} */ (cache.get(this._id));
        if (result)
            return result;
        var data = {
            id: this._id,
            name: this._name,
            kindId: this._kindId
        };
        result = new ReplayableResource(this, data);
        cache.put(this._id, result); // Put into the cache early to avoid loops.
        data.calls = this._calls.map(function(call) {
            return call.toReplayable(cache);
        });
        this._populateReplayableData(data, cache);
        var contextResource = this.contextResource();
        if (contextResource !== this)
            data.contextResource = Resource.toReplayable(contextResource, cache);
        return result;
    },

    /**
     * @param {!Object} data
     * @param {!Cache} cache
     */
    _populateReplayableData: function(data, cache)
    {
        // Do nothing. Should be overridden by subclasses.
    },

    /**
     * @param {!Object} data
     * @param {!Cache} cache
     * @return {!Resource}
     */
    replay: function(data, cache)
    {
        var resource = /** @type {Resource} */ (cache.get(data.id));
        if (resource)
            return resource;
        this._id = data.id;
        this._name = data.name;
        this._kindId = data.kindId;
        this._resourceManager = null;
        this._calls = [];
        this._boundResources = Object.create(null);
        this._wrappedObject = null;
        cache.put(data.id, this); // Put into the cache early to avoid loops.
        this._doReplayCalls(data, cache);
        console.assert(this._wrappedObject, "Resource should be reconstructed!");
        return this;
    },

    /**
     * @param {!Object} data
     * @param {!Cache} cache
     */
    _doReplayCalls: function(data, cache)
    {
        for (var i = 0, n = data.calls.length; i < n; ++i)
            this._calls.push(data.calls[